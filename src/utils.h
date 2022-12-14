#ifndef __UTILS__
#define __UTILS__

#include <string>
using namespace std;

// Function to check the string character by character
bool all_num(string& str)
{
    for (char const &c : str) {
        // using the isdigit() function
        if (isdigit(c) == 0)
          return false;
    }
    return true;
}

struct sort_descend {
    bool operator() (const pair<int,int> &left, const pair<int,int> &right) {
        return left.second > right.second;
    }
    bool operator() (const pair<int,pair<int,int>> &left, const pair<int,pair<int,int>> &right) {
        // return left.second > right.second;
        if (left.second.first > right.second.first) return true;
        if (left.second.first == right.second.first) return left.second.second > right.second.second;
        return false;
    }
};

#endif