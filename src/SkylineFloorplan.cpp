#include "SkylineFloorplan.h"

void SkylineFloorplan::floorplan(vector<int>& w, vector<int>& h, vector<int>& indices, SkylineHeuristic sh) {
    int HH = -1;
    int iter = indices.size();
    vector<bool> visited(iter, false);
    while (iter > 0) {
        Rectangle outer_best_rect;
        int outer_best_score1 = 0x7FFFFFFF, outer_best_score2 = 0x7FFFFFFF, outer_best_skyline_i = -1, outer_best_rect_i = -1, outer_best_rotate = 0;
        // Choose one best rectangle block in each of the following iteration
        for (int i : indices) {
            if (visited[i]) continue;
            int w_ = w[i], h_ = h[i];
            pair<Rectangle, int> rect_rotate;
            Rectangle rect;
            int rotate;
            // ------------- guillotine -------------
            if (guillotine) {
                rect_rotate = gspace.AddBlock(w_, h_, gh);
                rect = rect_rotate.first;
                rotate = rect_rotate.second;
                if (rect.h != 0) {
                    // x[i] = rect.x;
                    // y[i] = rect.y;
                    // r[i] = rotate;
                    outer_best_rect = rect;
                    outer_best_rotate = rotate;
                    outer_best_rect_i = i;
                    break;   // already found a solution during guillotine floorplanning
                }
            }
            // ------------- guillotine -------------
            int inner_best_score1 = 0x7FFFFFFF, inner_best_score2 = 0x7FFFFFFF, inner_best_skyline_i = -1;
            switch (sh) {
            case MinWasteFit:
                rect_rotate = AddBlockMinWaste(w_, h_, &inner_best_score2, &inner_best_score1, &inner_best_skyline_i);
                break;
            case BottomLeft: default:
                rect_rotate = AddBlockBottomLeft(w_, h_, &inner_best_score1, &inner_best_score2, &inner_best_skyline_i);
                break;
            }
            rect = rect_rotate.first;
            rotate = rect_rotate.second;
            if (rect.h != 0 && (inner_best_score1 < outer_best_score1 || (inner_best_score1 == outer_best_score1 && inner_best_score2 < outer_best_score2))) {
                outer_best_rect = rect;
                outer_best_rotate = rotate;
                outer_best_score1 = inner_best_score1;
                outer_best_score2 = inner_best_score2;
                outer_best_skyline_i = inner_best_skyline_i;
                outer_best_rect_i = i;
            }
            break;  // doing the loop only once, to use external sorting
        }
        assert(outer_best_rect_i != -1);
        if (outer_best_skyline_i != -1)
            newSkyline(outer_best_skyline_i, outer_best_rect);
            // cout << "iter = " << iter << endl;
            // cout << "len(skylines) = " << skylines.size() << endl;
            // cout << "x = " << skylines.back().x << ", y = " << skylines.back().y << ", w = " << skylines.back().w << endl;
        // Assign the block with best index
        x[outer_best_rect_i] = outer_best_rect.x;
        y[outer_best_rect_i] = outer_best_rect.y;
        r[outer_best_rect_i] = outer_best_rotate;
        // update the maximum height
        int curr_h = (outer_best_rotate) ? w[outer_best_rect_i] : h[outer_best_rect_i];
        if (curr_h + outer_best_rect.y > HH) HH = curr_h + outer_best_rect.y;
        // Pop the block with best index
        // indices.erase(indices.begin() + outer_best_rect_i);
        // w.erase(w.begin() + outer_best_rect_i);
        // h.erase(h.begin() + outer_best_rect_i);
        visited[outer_best_rect_i] = true;
        iter--;
    }
    
    string logfile = "./log/skyline-" + to_string(numBlocks) + "b-" + sorting_type + "-" + enum_to_string(sh);
    if (guillotine) {
        logfile += "-guillotine-";
        logfile += gspace.enum_to_string(gh);
    }
    logfile += ".log";
    cout << "log name: " << logfile << endl;
    generate_log(logfile, w, h, HH);
}

void SkylineFloorplan::GuillotineSkyline(int sindex, int w, int h, int y) {
    assert(h);
    const int rect_left = skylines[sindex].x;
    const int rect_right = rect_left + w;
    while ((size_t)sindex < skylines.size() && skylines[sindex].x < rect_right &&
            skylines[sindex].x + skylines[sindex].w > rect_left) {
        int _left = skylines[sindex].x;
        int _right = min(_left + skylines[sindex].w, rect_right);
        Rectangle rect = {
            .x = _left,
            .y = skylines[sindex].y,
            .w = _right - _left,
            .h = y - skylines[sindex].y
        };
        gspace.GetFreeRects().push_back(rect);
        sindex++;
    }
}

void SkylineFloorplan::newSkyline(int sindex, const Rectangle& rect) {
    GuillotineSkyline(sindex, rect.w, rect.h, rect.y);
    Skyline skyline {
        .x = rect.x,
        .y = rect.y + rect.h,
        .w = rect.w
    };
    skylines.insert(skylines.begin() + sindex, skyline);
    for (size_t i = sindex + 1; i < skylines.size(); i++) {
        if (skylines[i].x < skylines[i-1].x + skylines[i-1].w) {
            int shrink = skylines[i-1].x + skylines[i-1].w - skylines[i].x;
            skylines[i].x += shrink;
            skylines[i].w -= shrink;
            if (skylines[i].w <= 0) {
                skylines.erase(skylines.begin() + i);
                i--;
            }
            else break;
        }
        else break;
    }
    MergeSkylines();
}

void SkylineFloorplan::MergeSkylines() {
    for (size_t i = 0; i < skylines.size() - 1; i++) {
        if (skylines[i].y == skylines[i+1].y) {
            skylines[i].w += skylines[i+1].w;
            skylines.erase(skylines.begin() + i + 1);
            i--;
        }
    }
}

pair<Rectangle, int> SkylineFloorplan::AddBlockMinWaste(int w, int h, int* best_h, int* best_waste, int* best_i) {
    Rectangle rect;
    int rotate;
    memset(&rect, 0, sizeof(Rectangle));
    for (size_t i = 0; i < skylines.size(); i++) {
        int y = 0, waste_area = 0;
        if (RectangleFits(i, w, h, y, waste_area)) {
            if (waste_area < *best_waste || (waste_area == *best_waste && y + h < *best_h)) {
                *best_h = y + h;
                *best_waste = waste_area;
                *best_i = i;
                rect.x = skylines[i].x;
                rect.y = y;
                rect.w = w;
                rect.h = h;
                rotate = 0;
            }
        }
        if (RectangleFits(i, h, w, y, waste_area)) {
            if (waste_area < *best_waste || (waste_area == *best_waste && y + w < *best_h)) {
                *best_h = y + w;
                *best_waste = waste_area;
                *best_i = i;
                rect.x = skylines[i].x;
                rect.y = y;
                rect.w = h;
                rect.h = w;
                rotate = 1;
            }
        }
    }
    return make_pair(rect, rotate);
}

pair<Rectangle, int> SkylineFloorplan::AddBlockBottomLeft(int w, int h, int* best_w, int* best_h, int* best_i) {
    Rectangle rect;
    int rotate;
    memset(&rect, 0, sizeof(Rectangle));
    for (size_t i = 0; i < skylines.size(); i++) {
        int y = 0;
        if (RectangleFits(i, w, h, y)) {
            if (y + h < *best_h || (y + h == *best_h && skylines[i].w < *best_w)) {
                *best_w = skylines[i].w;
                *best_h = y + h;
                *best_i = i;
                rect.x = skylines[i].x;
                rect.y = y;
                rect.w = w;
                rect.h = h;
                rotate = 0;
            }
        }
        if (RectangleFits(i, h, w, y)) {
            if (y + w < *best_h || (y + w == *best_h && skylines[i].w < *best_w)) {
                *best_w = skylines[i].w;
                *best_h = y + w;
                *best_i = i;
                rect.x = skylines[i].x;
                rect.y = y;
                rect.w = h;
                rect.h = w;
                rotate = 1;
            }
        }
    }
    return make_pair(rect, rotate);
}

bool SkylineFloorplan::RectangleFits(int index, int w, int h, int &y) {
    assert(h); // to supress warning
    int x = skylines[index].x;
    if (x + w > chipwidth)
        return false;
    int w_left = w;
    int i = index;
    y = skylines[index].y;
    while(w_left > 0)
    {
        y = max(y, skylines[i].y);
        w_left -= skylines[i].w;
        i++;
    }
    return true;
}

bool SkylineFloorplan::RectangleFits(int index, int w, int h, int &y, int& waste_area) {
    bool fit = RectangleFits(index, w, h, y);
    if (fit) {
        int area = 0;
        const int rect_left = skylines[index].x;
        const int rect_right = rect_left + w;
        while ((size_t)index < skylines.size() && skylines[index].x < rect_right &&
                skylines[index].x + skylines[index].w > rect_left) {
            int _left = skylines[index].x;
            int _right = min(_left + skylines[index].w, rect_right);
            area += (_right - _left) * (y - skylines[index].y);
            index++;
        }
        waste_area = area;
    }
    return fit;
}

void SkylineFloorplan::generate_log(string& logfile, vector<int>& w, vector<int>& h, int H) {
    ofstream log_verbose(logfile);
    if (log_verbose.fail()) {
        cout << "[ERROR] File doesn't exist!!" << endl;
        exit(1);
    }
    
    cout << "\n=============== Results Output ===============\nOptimum Height: " << H << endl;

    log_verbose << chipwidth << " " << H << endl;
    for (int i = 0; i < numBlocks; i++) {
        cout << i << " : " << x[i] << " " << y[i] << " " << r[i] << endl;
        log_verbose << i << " : " << x[i] << " " << y[i] << " " << w[i] << " " << h[i] << " " << r[i] << endl;
    }
}
