#ifndef BOT_HPP
#define BOT_HPP

#include <vector>
#include <set>
#include <string>
#include "Coordinate.hpp"

using namespace std;

class Bot {
public:

    string command[12] = {"Halt", "Wait", "Flip", "SMove", "LMove", "Fission", "Fill", "Void", "FusionP", "FusionS", "GFill", "GVoid"};
    vector<vector<int> > hist_command;
    int identifier, parent, clock_time;
    Coordinate cur_pos, relative_pos, goal_pos;
    set<int> child;
    bool upper, corner;

    Bot(){}

    Bot(int id_, int par_) : identifier(id_), parent(par_){}

    Bot(int id_, set<int>& child_, Coordinate cur_pos_, int time_) : identifier(id_), parent(-1),
        clock_time(time_), cur_pos(cur_pos_), child(child_){}

    inline bool is_lower() {
        return upper;
    }

    inline bool is_corner() {
        return corner;
    }

    inline bool reached() {
        return (cur_pos == goal_pos);
    }

    void halt() {
        hist_command.emplace_back(clock_time, 0);
    }

    void wait() {
        hist_command.emplace_back(clock_time, 1);
    }

    void flip() {
        hist_command.emplace_back(clock_time, 2);
    }

    void smove(Coordinate dir) {

    }

    void lmove(Coordinate dir1, Coordinate dir2) {

    }

    void fission(Coordinate dir, int child_cnt) {

    }

    void fill(Coordinate dir) {

    }

    void void_(Coordinate dir) {

    }

    void fusionp(Coordinate dir) {

    }

    void fusions(Coordinate dir) {

    }

    void gfill(Coordinate dir, Coordinate dist) {

    }

    void gvoid(Coordinate dir, Coordinate dist) {

    }
};

#endif
