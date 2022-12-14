#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>
#include <ctype.h>
#include <algorithm>
#include "Floorplan.h"
#include "MILPFloorplan.h"
#include "ShelfFloorplan.h"
#include "GuillotineFloorplan.h"
#include "utils.h"

using namespace std;


int main(int argc, char *argv[]) {
    if (argc < 4) {
        cout << "[ERROR] Input format should be `<exe> <testcase> <algorithm> <sub-algorithm/time-limit> <sorting>`, e.g., `./floorplan 10.txt 0 7000`, or `.floorplan 10.txt 1 0 1`.";
        return 1;
    }
    ifstream testcase(argv[1]);
    if (testcase.fail()) {
        cout << "[ERROR] File doesn't exist!!" << endl;
        return 1;
    }
    string line, tempstr;
    // chipwidth
    int chipwidth = 0;
    testcase >> tempstr >> tempstr >> chipwidth;
    // numBlocks
    int numBlocks = 0;
    testcase >> tempstr >> tempstr >> numBlocks;
    assert(chipwidth != 0 && numBlocks != 0);
    cout << chipwidth << " " << numBlocks << endl;

    // arrays: indices, x, y
    int i = 0, MAX_H = 0;
    vector<int> indices(numBlocks), w(numBlocks), h(numBlocks);

    while (testcase && i < numBlocks) {
        testcase >> indices[i] >> tempstr >> w[i] >> h[i];
        // cout << i << " " << w[i] << " " << h[i] << endl;
        MAX_H += h[i];
        i++;
    }

    string _argv_2 = argv[2];
    string _argv_3 = argv[3];
    string _argv_4, _argv_5;
    int algorithm_type = 0;             // argv[2]
    int time_limit = 10, sub_type = 0;  // argv[3]
    int sorting = 0;                    // argv[4]
    string sorting_type = "none";
    vector<pair<int, int> > vp_one;
    vector<pair<int, pair<int, int> > > vp_two;

    // ---------------- Parsing Parameters ----------------
    if (all_num(_argv_2)) {
        algorithm_type = stoi(_argv_2);
    }
    else {
        cout << "[WARNING] the 2nd parameter is not an integer. Running default MILP floorplanning." << endl;
    }
    MILPFloorplan milp_fp(chipwidth, numBlocks, MAX_H);
    ShelfFloorplan shelf_fp(chipwidth, numBlocks, MAX_H);
    
    switch (algorithm_type) {
    case 0: // MILP Floorplanning
        if (all_num(_argv_3)) {
            time_limit = stoi(_argv_3);
        }
        else {
            cout << "[WARNING] the 3rd parameter is not an integer. Using default MILP time limit 10s." << endl;
        }
        cout << "> Type: MILP Floorplanning" << endl;
        cout << "> Time Limit: " << time_limit << " seconds" << endl;
        milp_fp.milp_floorplanning(w, h, time_limit);
        break;
    // -----------------------------------------------------------------    
    case 1: // Shelf Floorplanning
        if (argc > 3) {
            _argv_3 = argv[3];
            if (all_num(_argv_3)) sub_type = stoi(_argv_3);
            else cout << "[WARNING] Shelf the 3rd parameter is not an integer. Using default sub-algorithm NEXT-FIT." << endl;
            if (argc > 4) {
                _argv_4 = argv[4];
                if (all_num(_argv_4)) sorting = stoi(_argv_4);
                else cout << "[WARNING] Shelf the 4th parameter is not an integer. Using default no sorting." << endl;
            } else {
                cout << "[WARNING] Shelf sorting type not specified. Using default no sorting." << endl;
            }
        } 
        else {
            cout << "[WARNING] Shelf sub-algorithm and sorting type not specified. Using default sub-algorithm NEXT-FIT, and no sorting." << endl;
        }
        switch (sorting) {
        case 0: break;
        case 1: // Sort By Area
            for (int i = 0; i < numBlocks; i++)
                vp_one.push_back(make_pair(i, w[i] * h[i]));
            sort(vp_one.begin(), vp_one.end(), sort_descend());
            for (int i = 0; i < numBlocks; i++) {
                indices[i] = vp_one[i].first;
            }
            shelf_fp.set_sorting_type("area");
            break;
        case 2: // Sort by shorter first, longer second
            for (int i = 0; i < numBlocks; i++)
                vp_two.push_back(make_pair(i, make_pair(min(w[i], h[i]), max(w[i], h[i]))));
            sort(vp_two.begin(), vp_two.end(), sort_descend());
            for (int i = 0; i < numBlocks; i++) {
                indices[i] = vp_two[i].first;
                // cout << "short: " << vp_two[i].second.first << ", long: " << vp_two[i].second.second << ", index: " << indices[i] << endl;
            }
            shelf_fp.set_sorting_type("short-long");
            break;
        case 3: // Sort by longer first, shorter second
            for (int i = 0; i < numBlocks; i++)
                vp_two.push_back(make_pair(i, make_pair(max(w[i], h[i]), min(w[i], h[i]))));
            sort(vp_two.begin(), vp_two.end(), sort_descend());
            for (int i = 0; i < numBlocks; i++) {
                indices[i] = vp_two[i].first;
                // cout << "long: " << vp_two[i].second.first << ", short: " << vp_two[i].second.second << ", index: " << indices[i] << endl;
            }
            shelf_fp.set_sorting_type("long-short");
            break;
        case 4: // Sort By Perimeter
            for (int i = 0; i < numBlocks; i++)
                vp_one.push_back(make_pair(i, w[i] + h[i]));
            sort(vp_one.begin(), vp_one.end(), sort_descend());
            for (int i = 0; i < numBlocks; i++) {
                indices[i] = vp_one[i].first;
            }
            shelf_fp.set_sorting_type("perimeter");
            break;
        case 5: // Sort By side difference
            for (int i = 0; i < numBlocks; i++)
                vp_one.push_back(make_pair(i, abs(w[i] - h[i])));
            sort(vp_one.begin(), vp_one.end(), sort_descend());
            for (int i = 0; i < numBlocks; i++) {
                indices[i] = vp_one[i].first;
            }
            shelf_fp.set_sorting_type("side-diff-descend");
            break;
        default: break;
        }
        // Guillotine type (if any)
        switch (sub_type)
        {
        case 4: case 5: case 6: case 7: shelf_fp.set_guillotine(true, GuillotineFloorplan::RectBestAreaFit); break;
        case 8: case 9: case 10: case 11: shelf_fp.set_guillotine(true, GuillotineFloorplan::RectBestShortSideFit); break;
        case 12: case 13: case 14: case 15: shelf_fp.set_guillotine(true, GuillotineFloorplan::RectBestLongSideFit); break;        
        default:
            break;
        }

        switch (sub_type)
        {
        case 0: case 4: case 8: case 12: shelf_fp.bin_shelf_next_fit_floorplanning(w, h, indices); break;
        case 1: case 5: case 9: case 13: shelf_fp.bin_shelf_first_fit_floorplanning(w, h, indices); break;
        case 2: case 6: case 10: case 14: shelf_fp.bin_shelf_best_width_fit_floorplanning(w, h, indices); break;
        case 3: case 7: case 11: case 15: shelf_fp.bin_shelf_best_height_fit_floorplanning(w, h, indices); break;
        default:
            cout << "[WARNING] Shelf Wrong sub-algorithm value. Using default sub-algorithm NEXT-FIT." << endl;
            shelf_fp.bin_shelf_next_fit_floorplanning(w, h, indices);
            break;
        }
        break;
    // -----------------------------------------------------------------
    case 2: break;
    // -----------------------------------------------------------------
    default:
        exit(1);
        break;
    }
    // MILPFloorplan milp_fp(chipwidth, numBlocks, MAX_H);
    // milp_fp.milp_floorplanning(w, h, 3000);
    // ShelfFloorplan shelf_fp(chipwidth, numBlocks, MAX_H);
    // shelf_fp.bin_shelf_next_fit_floorplanning(w, h, indices);
    // shelf_fp.bin_shelf_first_fit_floorplanning(w, h, indices);
    // shelf_fp.bin_shelf_best_width_fit_floorplanning(w, h, indices);
    // shelf_fp.bin_shelf_best_height_fit_floorplanning(w, h, indices);

    return 0;
}