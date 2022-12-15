#ifndef __SKYLINEFP__
#define __SKYLINEFP__

#include <iostream>
#include <fstream>
#include <cassert>
#include <cmath>
#include <vector>
#include <cstring>
#include "Floorplan.h"
#include "GuillotineFloorplan.h"

using namespace std;

class SkylineFloorplan : public Floorplan {
private:
    struct Skyline {
        int x;
        int y;
        int w;
    };
    vector<Skyline> skylines;
    vector<int> x, y, r;
    string sorting_type;
    bool guillotine;
    GuillotineFloorplan gspace;
    GuillotineFloorplan::GHeuristic gh;
public:
	enum SkylineHeuristic
	{
		BottomLeft,
		MinWasteFit
	};
    inline string enum_to_string(SkylineHeuristic sh) {
        switch (sh) {
        case BottomLeft: return "BottomLeft"; break;
        case MinWasteFit: return "MinWasteFit"; break;
        default: return ""; break;
        }
    }
    SkylineFloorplan(int w, int N, int H) : Floorplan(w, N, H) {
        sorting_type = "none";
        guillotine = false;
        gh = GuillotineFloorplan::RectBestAreaFit;
        Skyline skyline = {
            .x = 0,
            .y = 0,
            .w = chipwidth
        };
        skylines.push_back(skyline);
        for (int _ = 0; _ < numBlocks; _++) {
            x.push_back(0);
            y.push_back(0);
            r.push_back(0);
        }
    }
    void floorplan(vector<int>& w, vector<int>& h, vector<int>& indices, SkylineHeuristic sh);
    void GuillotineSkyline(int sindex, int w, int h, int y);
    void newSkyline(int sindex, const Rectangle& rect);
    void MergeSkylines();
    pair<Rectangle, int> AddBlockMinWaste(int w, int h, int* best_h, int* best_waste, int* best_i);
    pair<Rectangle, int> AddBlockBottomLeft(int w, int h, int* best_w, int* best_h, int* best_i);
    bool RectangleFits(int index, int w, int h, int &y);
    bool RectangleFits(int index, int w, int h, int &y, int& waste_area);

    // Utils
    inline void set_sorting_type(const string& type) {
        cout << "Sorting in decreasing order by: " << type << endl;
        sorting_type = type;
    }
    inline void set_guillotine(bool gtype, GuillotineFloorplan::GHeuristic htype) {
        guillotine = gtype;
        gh = htype;
    }
    void generate_log(string& logfile, vector<int>& w, vector<int>& h, int H);
};

#endif