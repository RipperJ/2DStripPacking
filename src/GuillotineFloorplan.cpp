#include "GuillotineFloorplan.h"

pair<Rectangle, int> GuillotineFloorplan::AddBlock(int w, int h, GHeuristic gh) {
    int free_index = 0;
    pair<Rectangle, int> temp_pair = AddBlockByHeuristic(w, h, gh, &free_index);
    Rectangle rect = temp_pair.first;
    int rotate = temp_pair.second;
    if (rect.h == 0) return make_pair(rect, 0);
    SplitFreeRectByHeuristic(free_rects[free_index], rect); // Currently only by min-area
    free_rects.erase(free_rects.begin() + free_index);
    MergeFreeRects();   // Merge by default
    return make_pair(rect, rotate);
}

void GuillotineFloorplan::MergeFreeRects() {
    for (size_t i = 0; i < free_rects.size(); i++) {
        for (size_t j = i + 1; j < free_rects.size(); j++) {
            if (free_rects[i].w == free_rects[j].w && free_rects[i].x == free_rects[j].x) {
                if (free_rects[i].y == free_rects[j].y + free_rects[j].h) {
                    free_rects[i].y -= free_rects[j].h;
                    free_rects[i].h += free_rects[j].h;
                    free_rects.erase(free_rects.begin() + j);
                    j--;
                }
                else if (free_rects[i].y + free_rects[i].h == free_rects[j].y) {
                    free_rects[i].h += free_rects[j].h;
                    free_rects.erase(free_rects.begin() + j);
                    j--;
                }
            }
            else if (free_rects[i].h == free_rects[j].h && free_rects[i].y == free_rects[j].y) {
                if (free_rects[i].x == free_rects[j].x + free_rects[j].w) {
                    free_rects[i].x -= free_rects[j].w;
                    free_rects[i].w += free_rects[j].w;
                    free_rects.erase(free_rects.begin() + j);
                    j--;
                }
                else if (free_rects[i].x + free_rects[i].w == free_rects[j].x) {
                    free_rects[i].w += free_rects[j].w;
                    free_rects.erase(free_rects.begin() + j);
                    j--;
                }
            }
        }
    }
}

pair<Rectangle, int> GuillotineFloorplan::AddBlockByHeuristic(int w, int h, GHeuristic gh, int* free_index) {
    Rectangle rect;
    int rotate;
    memset(&rect, 0, sizeof(Rectangle));
    int best_score = 0x7FFFFFFF;
    for (size_t i = 0; i < free_rects.size(); i++)         {
        // If this is a perfect fit upright, choose it immediately.
        if (w == free_rects[i].w && h == free_rects[i].h) {
            rect.x = free_rects[i].x;
            rect.y = free_rects[i].y;
            rect.w = w;
            rect.h = h;
            *free_index = i;
            rotate = 0;
            break;
        }
        // If this is a perfect fit sideways, choose it.
        else if (h == free_rects[i].w && w == free_rects[i].h)
        {
            rect.x = free_rects[i].x;
            rect.y = free_rects[i].y;
            rect.w = h;
            rect.h = w;
            *free_index = i;
            rotate = 1;
            break;
        }
        // Does the rectangle fit upright?
        else if (w <= free_rects[i].w && h <= free_rects[i].h)
        {
            int score = ScoreByHeuristic(w, h, free_rects[i], gh);

            if (score < best_score)
            {
                rect.x = free_rects[i].x;
                rect.y = free_rects[i].y;
                rect.w = w;
                rect.h = h;
                best_score = score;
                *free_index = i;
                rotate = 0;
            }
        }
        // Does the rectangle fit sideways?
        else if (h <= free_rects[i].w && w <= free_rects[i].h)
        {
            int score = ScoreByHeuristic(h, w, free_rects[i], gh);

            if (score < best_score)
            {
                rect.x = free_rects[i].x;
                rect.y = free_rects[i].y;
                rect.w = h;
                rect.h = w;
                best_score = score;
                *free_index = i;
                rotate = 1;
            }
        }
    }
    return make_pair(rect, rotate);
}

int GuillotineFloorplan::ScoreByHeuristic(int w, int h, const Rectangle& free_rect, GHeuristic gh) {
    int score = 0x7FFFFFFF;
    switch (gh) {
    case RectBestAreaFit: score = free_rect.w * free_rect.h - w * h; break;
    case RectBestShortSideFit: score = min(abs(free_rect.w - w), abs(free_rect.h - h)); break;
    case RectBestLongSideFit: score = max(abs(free_rect.w - w), abs(free_rect.h - h)); break;
    default: break;
    }
    return score;
}

void GuillotineFloorplan::SplitFreeRectByHeuristic(const Rectangle& free_rect, const Rectangle& placed_rect) {
    // TODO: Add more heuristics, currently only min area
    const int w = free_rect.w - placed_rect.w;
    const int h = free_rect.h - placed_rect.h;
    bool split_horizontal = (placed_rect.w * h > w * placed_rect.h);    // Min area

    Rectangle bottom = {
        .x = free_rect.x,
        .y = free_rect.y + placed_rect.h,
        .w = (split_horizontal) ? free_rect.w : placed_rect.w,
        .h = free_rect.h - placed_rect.h
    };
    Rectangle right = {
        .x = free_rect.x + placed_rect.w,
        .y = free_rect.y,
        .w = free_rect.w - placed_rect.w,
        .h = (split_horizontal) ? placed_rect.h : free_rect.h
    };

    if (bottom.w > 0 && bottom.h > 0) free_rects.push_back(bottom);
    if (right.w > 0 && right.h > 0) free_rects.push_back(right);
}