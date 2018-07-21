#ifndef GAME_HPP
#define GAME_HPP

#include "UnionFind.hpp"
#include "Coordinate.hpp"
#include "Bot.hpp"
#define rep(i,n) for(int i=0;i<(int)(n);++i)

class Game {
public:
    static const int MAX_BORING_SIZE = 10;
    const int BLOCK_SIZE = 28;
    const int MAX_SEED = 40;
    const int MAX_SMOVE = 15;
    const Coordinate INIT_POS = Coordinate(0, 0, 0);
    const Coordinate dx = Coordinate(1, 0, 0);
    const Coordinate dy = Coordinate(0, 1, 0);
    const Coordinate dz = Coordinate(0, 0, 1);
    vector<Bot> botset;

private:
    int resol, cube, voxel_count, bot_count, clock_time;
    bool* field;
    long long energy_sum;
    int leftx, lefty, leftz, rightx, righty, rightz, rangex, rangey, rangez;
    int boringx[MAX_BORING_SIZE], boringz[MAX_BORING_SIZE];

    void pre_calc() {
        int indexx = 0, curx = leftx-1;
        for(; curx < rightx; indexx++){
            boringx[indexx] = curx;
            curx += BLOCK_SIZE+1;
        }
        boringx[indexx] = rightx+1;
        int indexz = 0, curz = leftz-1;
        for(; curz < rightz; indexz++){
            boringz[indexz] = curz;
            curx += BLOCK_SIZE+1;
        }
        boringz[indexz] = rightz+1;
    }

    void init_bot_set() {

    }

    void pre_conditioning() {
        pre_calc();
        init_bot_set();
    }

    void alloc_target() {

    }

    bool part_end_check() {

    }

    bool boring() {
        alloc_target();
        bool lower_end = false, upper_end = true;
        while(!lower_end || !upper_end) {
            set<Coordinate> bot_filled;
            lower_end |= lower_action(bot_filled);
            upper_end |= upper_action(bot_filled);
        }
        return part_end_check();
    }

    bool erase() {

    }

    bool lower_action(set<Coordinate>& ) {
        bool lower_end = true;
        for(auto& bot : botset) {
            if(bot.is_lower()) {
                if(bot.reached()) {
                    bot.halt();
                }else{
                    lower_end = false;
                    Coordinate next_pos = bot.cur_pos - dz;
                    int next_pos_int = Coordinate::ctoi(next_pos, resol);
                    if(field[next_pos_int]){
                        bot.void_(-dz);
                        field[next_pos_int] = false;
                    }else{
                        for(int adv = 1; adv <= MAX_SMOVE; adv++){
                            if(field[Coordinate::ctoi(next_pos - dz, resol)]) break;
                            next_pos -= dz;
                        }
                        bot.smove(next_pos - bot.cur_pos);
                        bot.cur_pos = next_pos;
                    }
                }
            }
        }
        return lower_end;
    }

    bool upper_action() {
        for(auto& bot : botset){
            if(!bot.is_lower()){
                if(bot.reached()) {
                    bot.halt();
                }else{

                }
            }
        }
    }

    void upper_move() {

    }

    void lower_move() {

    }

    bool all_end_check() {

    }

    void move() {
        upper_move();
        lower_move();
    }

    void swap_upper_lower() {

    }

    void return_() {

    }

    void reverse() {
        UnionFind voxel_component(cube);
        for(int x = 0; x < resol-1; x++) {
            for(int z = 0; z < resol; z++) {
                voxel_component.unite(Coordinate::tritoi(x, 0, z, resol),
                    Coordinate::tritoi(x+1, 0, z, resol));
            }
        }
    }

public:

    Game() : voxel_count(0), bot_count(0), clock_time(0), energy_sum(0LL),
        leftx(INF), lefty(INF), leftz(INF), rightx(0), righty(0), rightz(0){}

    void build(int R_) {
        resol = R_;
        cube = resol * resol * resol;
        field = new bool[cube];
        rep(i, MAX_SEED){
            if(i == 0){
                set<int> init_child;
                for(int j=1; j < MAX_SEED; j++){
                    init_child.insert(j);
                }
                botset.emplace_back(0, init_child, INIT_POS, 0);
                bot_count++;
            }else{
                botset.emplace_back(i, 0);
            }
        }
    }

    inline void voxel_set(int x_, int y_, int z_) {
        leftx = min(leftx, x_), lefty = min(lefty, y_), leftz = min(leftz, z_);
        rightx = max(rightx, x_), righty = max(righty, y_), rightz = max(rightz, z_);
        field[Coordinate::tritoi(x_, y_, z_, resol)] = true;
    }

    void play() {
        pre_conditioning();
        while(true){
            do {
                while(boring()){};
            } while(erase());
            if(all_end_check()) break;
            move();
            swap_upper_lower();
        }
        return_();
    }

    void determine() {
        reverse();
    }

    inline int cur_time() {
        return clock_time;
    }
};

#endif
