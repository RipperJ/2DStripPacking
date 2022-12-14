#ifndef __FLOORPLAN__
#define __FLOORPLAN__

#include <string>

using namespace std;

struct Rectangle {
    int x;
    int y;
    int w;
    int h;
};

class Floorplan {
protected:
    int chipwidth;
    int numBlocks;
    int MAX_H;
public:
    Floorplan(int w, int N, int H);
    // virtual void generate_log(string& logfile);
    // void min_area_first_random_sizing_floorplanning(vector<int>& w, vector<int>& h);
    // void sa_floorplanning() {
    // }
};

#endif