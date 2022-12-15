#ifndef __GUILLOTINEFP__
#define __GUILLOTINEFP__

#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <cstring>
#include "Floorplan.h"

using namespace std;

class GuillotineFloorplan {
private:
    vector<Rectangle> free_rects;
public:
	enum GHeuristic
	{
		RectBestAreaFit,
		RectBestShortSideFit, 
		RectBestLongSideFit
	};
    inline string enum_to_string(GHeuristic gh) {
        switch (gh) {
        case RectBestAreaFit: return "RectBestAreaFit"; break;
        case RectBestShortSideFit: return "RectBestShortSideFit"; break;
        case RectBestLongSideFit: return "RectBestLongSideFit"; break;
        default: return ""; break;
        }
    }
    // GuillotineFloorplan(int w, int N, int H) : Floorplan(w, N, H) {
    //     Rectangle R;
    //     R.x = 0;
    //     R.y = 0;
    //     R.w = w;
    //     R.h = 0x7FFFFFFF;
    //     free_rects.push_back(R);
    // }
    GuillotineFloorplan() {}
    pair<Rectangle, int> AddBlock(int w, int h, GHeuristic gh);
    void MergeFreeRects();
    pair<Rectangle, int> AddBlockByHeuristic(int w, int h, GHeuristic gh, int* free_index);
    int ScoreByHeuristic(int w, int h, const Rectangle& free_rect, GHeuristic gh);
    void SplitFreeRectByHeuristic(const Rectangle& free_rect, const Rectangle& placed_rect);
    inline vector<Rectangle>& GetFreeRects() {
        return free_rects;
    }
};

#endif