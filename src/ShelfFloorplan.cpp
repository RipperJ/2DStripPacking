#include "ShelfFloorplan.h"

void ShelfFloorplan::newShelf(int initHeight) {
    if (shelves.size() > 0) {
        curr_Y += shelves.back().shelf_H;
    }
    Shelf shelf;
    shelf.curr_X = 0;
    shelf.curr_Y = curr_Y;
    shelf.shelf_H = initHeight;
    shelves.push_back(shelf);
}

bool ShelfFloorplan::BlockFitInShelf(const Shelf& shelf, int w, int h) {
    if (
        (shelf.curr_X + w <= chipwidth && shelf.curr_X + h <= chipwidth) ||
        (!(shelf.curr_X + w > chipwidth && shelf.curr_X + h > chipwidth) && max(w, h) <= shelf.shelf_H)
        ) 
        return true;
    else return false;
}

void ShelfFloorplan::BlockRotateOnShelf(const Shelf& shelf, int& w, int& h, int i) {
    /*
    Rules:
    0. w < h < shelf_H -> no rotate
    1. h < w < shelf_H -> rotate
    2. w < shelf_H < h -> rotate
    3. h < shelf_H < w -> no rotate
    4. shelf_H < w < h -> rotate
    5. shelf_H < h < w -> no rotate

    6. If the rectangle is the first rectangle on a new open shelf, store it sideways. This is to minimize the height of the new shelf. [rotate]
    7. If the longer side cannot fit into the current slot, then use the shorter side. [rotate]
        ! The pre-condition is that "BlockFitInShelf" is already checked! Which means that the shorter side is guaranteed to fit!
    */
    if (
        // Rule 7
        (w > h && w > chipwidth - shelf.curr_X) ||
        // Rule 1
        (w > h && w < shelf.shelf_H) ||
        // Rule 2, Rule 4, Rule 6
        (w < h && h > shelf.shelf_H && h <= chipwidth - shelf.curr_X)
        ) {
        swap(w, h);
        r[i] = !r[i];
    }
}

void ShelfFloorplan::BlockAssignToShelf(Shelf& shelf, int w, int h, int i) {
    assert(BlockFitInShelf(shelf, w, h));
    BlockRotateOnShelf(shelf, w, h, i);
    x[i] = shelf.curr_X;
    y[i] = shelf.curr_Y;

    // For guillotine
    Rectangle rect {
        .x = shelf.curr_X,
        .y = shelf.curr_Y,
        .w = w,
        .h = h
    };
    shelf.existing_blocks.push_back(rect);

    shelf.curr_X += w;
    assert(shelf.curr_X <= chipwidth);
    shelf.shelf_H = max(shelf.shelf_H, h);
    // cout << "i: " << i << ", x: " << x[i] << ", y: " << y[i] << ", shelf_H: " << shelf.shelf_H << endl;
}

void ShelfFloorplan::bin_shelf_next_fit_floorplanning(vector<int>& w, vector<int>& h, vector<int>& indices) {
    // for (int i = 0; i < numBlocks; i++) {
    for (int i : indices) {
        /*
        // Rules 1, 2, 4, and 6
        int width = w[i], height = h[i];
        r[i] = 0;
        if ((w[i] > h[i] && w[i] < shelf_H) || (w[i] < h[i] && h[i] > shelf_H)) {
            r[i] = 1;
            swap(width, height);
        }
        // If cannot fit into the end of the current shelf
        if (curr_X + width > chipwidth) {
            curr_X = 0;
            curr_Y += shelf_H;
            shelf_H = 0;
            // Rule 6
            if (width < height) {
                swap(width, height);
                r[i] = !r[i];
            }
        }
        // If the rectangle cannot fit in the current orientation, rotate
        if (width > chipwidth) {
            swap(width, height);
            r[i] = !r[i];
        }
        //! If rotation still fails, report an error!!!
        if (width > chipwidth) {
            cout << "[ERROR] Block cannot fit into the strip!" << endl;
            cout << "X: " << curr_X << ", Y: " << curr_Y << ", W: " << w[i] << ", H: " << h[i] << ", rotate: " << r[i] << endl;
            exit(1);
        }
        // Log block position and Update shelf info
        x[i] = curr_X;
        y[i] = curr_Y;
        curr_X += width;
        shelf_H = max(shelf_H, height);
        */
        if (guillotine) {
            pair<Rectangle, int> rect_rotate = gspace.AddBlock(w[i], h[i], gh);
            Rectangle rect = rect_rotate.first;
            int rotate = rect_rotate.second;
            if (rect.h != 0) {
                x[i] = rect.x;
                y[i] = rect.y;
                r[i] = rotate;
                continue;   // already found a solution during guillotine floorplanning
            }
        }
        if (!BlockFitInShelf(shelves.back(), w[i], h[i])) {
            // cout << "current shelf_H: " << shelves.back().shelf_H << endl;
            if (guillotine) GuillotineShelf(shelves.back());
            newShelf(min(w[i], h[i]));
        }
        BlockAssignToShelf(shelves.back(), w[i], h[i], i);
    }
    
    string logfile = "./log/shelfNF-" + to_string(numBlocks) + "b-" + sorting_type;
    if (guillotine) {
        logfile += "-guillotine-";
        logfile += gspace.enum_to_string(gh);
    }
    logfile += ".log";
    cout << "log name: " << logfile << endl;
    generate_log(logfile, w, h, curr_Y + shelves.back().shelf_H);
}

void ShelfFloorplan::bin_shelf_first_fit_floorplanning(vector<int>& w, vector<int>& h, vector<int>& indices) {
    // for (int i = 0; i < numBlocks; i++) {
    for (int i : indices) {
        bool found = false;
        size_t j = 0;
        if (guillotine) {
            pair<Rectangle, int> rect_rotate = gspace.AddBlock(w[i], h[i], gh);
            Rectangle rect = rect_rotate.first;
            int rotate = rect_rotate.second;
            if (rect.h != 0) {
                x[i] = rect.x;
                y[i] = rect.y;
                r[i] = rotate;
                continue;   // already found a solution during guillotine floorplanning
            }
        }
        for (; j < shelves.size(); j++) {
            if (BlockFitInShelf(shelves[j], w[i], h[i])) {
                found = true;
                break;
            }
        }
        if (found == false) {
            if (guillotine) GuillotineShelf(shelves.back());
            newShelf(min(w[i], h[i]));
        }
        BlockAssignToShelf(shelves[j], w[i], h[i], i);
    }
    
    string logfile = "./log/shelfFF-" + to_string(numBlocks) + "b-" + sorting_type;
    if (guillotine) {
        logfile += "-guillotine-";
        logfile += gspace.enum_to_string(gh);
    }
    logfile += ".log";
    cout << "log name: " << logfile << endl;
    generate_log(logfile, w, h, curr_Y + shelves.back().shelf_H);
}

void ShelfFloorplan::bin_shelf_best_width_fit_floorplanning(vector<int>& w, vector<int>& h, vector<int>& indices) {
    // for (int i = 0; i < numBlocks; i++) {
    for (int i : indices) {
        int best_j = -1;
        int width_left = 0x7FFFFFFF;
        int w_ = w[i], h_ = h[i];
        if (guillotine) {
            pair<Rectangle, int> rect_rotate = gspace.AddBlock(w_, h_, gh);
            Rectangle rect = rect_rotate.first;
            int rotate = rect_rotate.second;
            if (rect.h != 0) {
                x[i] = rect.x;
                y[i] = rect.y;
                r[i] = rotate;
                continue;   // already found a solution during guillotine floorplanning
            }
        }
        for (size_t j = 0; j < shelves.size(); j++) {
            BlockRotateOnShelf(shelves[j], w_, h_, i);
            if (BlockFitInShelf(shelves[j], w_, h_)) {
                if (width_left > chipwidth - shelves[j].curr_X) {
                    width_left = chipwidth - shelves[j].curr_X;
                    best_j = (int)j;
                }
            }
        }
        if (best_j == -1) {
            if (guillotine) GuillotineShelf(shelves.back());
            best_j = shelves.size();
            newShelf(min(w_, h_));
        }
        BlockAssignToShelf(shelves[(size_t)best_j], w_, h_, i);
    }
    
    string logfile = "./log/shelfBWF-" + to_string(numBlocks) + "b-" + sorting_type;
    if (guillotine) {
        logfile += "-guillotine-";
        logfile += gspace.enum_to_string(gh);
    }
    logfile += ".log";
    cout << "log name: " << logfile << endl;
    generate_log(logfile, w, h, curr_Y + shelves.back().shelf_H);
}

void ShelfFloorplan::bin_shelf_best_height_fit_floorplanning(vector<int>& w, vector<int>& h, vector<int>& indices) {
    // for (int i = 0; i < numBlocks; i++) {
    for (int i : indices) {
        int best_j = -1;
        int height_left = 0x7FFFFFFF;
        int w_ = w[i], h_ = h[i];
        if (guillotine) {
            pair<Rectangle, int> rect_rotate = gspace.AddBlock(w_, h_, gh);
            Rectangle rect = rect_rotate.first;
            int rotate = rect_rotate.second;
            if (rect.h != 0) {
                x[i] = rect.x;
                y[i] = rect.y;
                r[i] = rotate;
                continue;   // already found a solution during guillotine floorplanning
            }
        }
        for (size_t j = 0; j < shelves.size(); j++) {
            BlockRotateOnShelf(shelves[j], w_, h_, i);
            if (BlockFitInShelf(shelves[j], w_, h_)) {
                if (height_left > max(shelves[j].shelf_H - h_, 0)) {
                    height_left = max(shelves[j].shelf_H - h_, 0);
                    assert(height_left >= 0);
                    best_j = (int)j;
                }
            }
        }
        if (best_j == -1) {
            if (guillotine) GuillotineShelf(shelves.back());
            best_j = shelves.size();
            newShelf(min(w_, h_));
        }
        BlockAssignToShelf(shelves[(size_t)best_j], w_, h_, i);
    }
    
    string logfile = "./log/shelfBHF-" + to_string(numBlocks) + "b-" + sorting_type;
    if (guillotine) {
        logfile += "-guillotine-";
        logfile += gspace.enum_to_string(gh);
    }
    logfile += ".log";
    cout << "log name: " << logfile << endl;
    generate_log(logfile, w, h, curr_Y + shelves.back().shelf_H);
}

void ShelfFloorplan::generate_log(string& logfile, vector<int>& w, vector<int>& h, int H) {
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

void ShelfFloorplan::GuillotineShelf(Shelf& shelf) {
    vector<Rectangle>& free_rects = gspace.GetFreeRects();
    for (size_t i = 0; i < shelf.existing_blocks.size(); i++) {
        const Rectangle& old_rect = shelf.existing_blocks[i];
        Rectangle rect = {
            .x = old_rect.x,
            .y = old_rect.y + old_rect.h,
            .w = old_rect.w,
            .h = shelf.shelf_H - old_rect.h
        };
        if (rect.h > 0) free_rects.push_back(rect);
    }
    shelf.existing_blocks.clear();
    Rectangle rect = {
        .x = shelf.curr_X,
        .y = shelf.curr_Y,
        .w = chipwidth - shelf.curr_X,
        .h = shelf.shelf_H
    };
    if (rect.w > 0) free_rects.push_back(rect);
    shelf.curr_X = chipwidth;
    gspace.MergeFreeRects();
}