#ifndef __SHELFFLOORPLAN__
#define __SHELFFLOORPLAN__

#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <cassert>
#include "Floorplan.h"

using namespace std;

struct Shelf {
    int curr_X;
    int curr_Y;
    int shelf_H;
};

class ShelfFloorplan : public Floorplan {
private:
    int curr_X;
    int curr_Y;
    int shelf_H;    // For single shelf, e.g., in next-fit algorithm
    vector<Shelf> shelves;
    vector<int> x, y, r;
    string sorting_type;
public:
    ShelfFloorplan(int w, int N, int H) : Floorplan(w, N, H) {
        curr_X = 0;
        curr_Y = 0;
        shelf_H = 0;
        sorting_type = "none";
        for (int _ = 0; _ < numBlocks; _++) {
            x.push_back(0);
            y.push_back(0);
            r.push_back(0);
        }
        newShelf(0);
    }

    void newShelf(int initHeight);
    bool BlockFitInShelf(const Shelf& shelf, int w, int h);
    void BlockRotateOnShelf(const Shelf& shelf, int& w, int& h, int i);
    void BlockAssignToShelf(Shelf& shelf, int w, int h, int i);
    void bin_shelf_next_fit_floorplanning(vector<int>& w, vector<int>& h, vector<int>& indices);
    void bin_shelf_first_fit_floorplanning(vector<int>& w, vector<int>& h, vector<int>& indices);
    void bin_shelf_best_width_fit_floorplanning(vector<int>& w, vector<int>& h, vector<int>& indices);
    void bin_shelf_best_height_fit_floorplanning(vector<int>& w, vector<int>& h, vector<int>& indices);
    
    inline void set_sorting_type(const string& type) {
        cout << "Sorting in decreasing order by: " << type << endl;
        sorting_type = type;
    }
    void generate_log(string& logfile, vector<int>& w, vector<int>& h, int H);
};

#endif