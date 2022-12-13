#ifndef __MILPFLOORPLAN__
#define __MILPFLOORPLAN__

#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include "gurobi_c++.h"
#include "Floorplan.h"

using namespace std;

class MILPFloorplan : public Floorplan {
public:
    MILPFloorplan(int w, int N, int H) : Floorplan(w, N, H) {}
    void milp_floorplanning(vector<int>& w, vector<int>& h, int timeout);
};

#endif