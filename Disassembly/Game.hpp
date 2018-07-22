#ifndef GAME_HPP
#define GAME_HPP

#include "UnionFind.hpp"
#include "Coordinate.hpp"
#include "Bot.hpp"
#define rep(i,n) for(int i=0;i<(int)(n);++i)
#define INF 1000000007

class Game {
public:
    // 座標の最大値
    static const int MAX_RESOL = 250;
    // 遠方座標の最大値
    static const int MAX_FAR_DIST = 30;
    // ブロックの大きさ
    static const int BLOCK_SIZE = MAX_FAR_DIST - 2;
    // x および z 方向上に掘る穴の最大数
    static const int MAX_BORING_SIZE = ceil(MAX_RESOL / BLOCK_SIZE) + 1;
    // 最大のbot数
    static const int MAX_SEED = 40;
    // SMove での最大移動距離
    static const int MAX_SMOVE = 15;
    // LMove での最大移動距離
    static const int MAX_LMOVE = 5;
    const Coordinate INIT_POS = Coordinate(0, 0, 0);
    const Coordinate dx = Coordinate(1, 0, 0);
    const Coordinate dy = Coordinate(0, 1, 0);
    const Coordinate dz = Coordinate(0, 0, 1);
    vector<Bot> botset;

private:

    // resolution の値, resolution^3, voxelの数, botの数, 時間(ステップ数)
    int resol, cube, voxel_count, bot_count, clock_time;
    // 現在 voxel があるかないか
    bool* field;
    // エネルギーの合計
    long long energy_sum;
    // 直方体の範囲を表す変数
    int leftx, lefty, leftz, rightx, righty, rightz, rangex, rangey, rangez;
    // 現在考えようとしている直方体の x, y, z 方向の長さ
    int cur_lenx, cur_leny, cur_lenz;
    // 掘る位置の x 座標と y 座標
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

    bool erase_check() {
        for(auto& bot : botset){
            if(!bot.reached()) return false;
        }
        return true;
    }

    void boring() {
        alloc_target();
        while(true) {
            set<Coordinate> bot_filled;
            lower_action(bot_filled);
            upper_action(bot_filled);
            if(erase_check()) return;
        }
    }

    bool erase_cubioid() {
        bool end_judge = false;
        for(auto& bot : botset) {
            if(bot.is_lower() && bot.is_corner()) {
                bot.gvoid(dy, Coordinate(cur_lenx-1, cur_leny-1, cur_lenz-1)*(-bot.relative_pos));
                if(bot.cur_pos.y == 0) {
                    end_judge = true;
                }
            }else if(!bot.is_lower() && bot.is_corner()){
                bot.gvoid(-dy, Coordinate(cur_lenx-1, -(cur_leny-1), cur_lenz-1)*(-bot.relative_pos));
            }else{
                bot.halt();
            }
        }
        return end_judge;
    }

    void erase_rectangle() {
        for(auto& bot : botset) {
            if(bot.is_lower()) {
                if(bot.relative_pos.z < 0){
                    bot.gvoid(dz, Coordinate(cur_lenx-1, 0, cur_lenz-3)*(-bot.relative_pos));
                }else{
                    bot.gvoid(-dz, Coordinate(cur_lenx-1, 0, -(cur_lenz-3))*(-bot.relative_pos));
                }
            }
        }
    }

    void erase_line() {
        for(auto& bot : botset) {
            if(bot.is_lower()) {
                if(bot.relative_pos.z < 0){
                    bot.gvoid(dz, Coordinate(cur_lenx-1, 0, cur_lenz-3)*(-bot.relative_pos));
                }else{
                    bot.gvoid(-dz, Coordinate(cur_lenx-1, 0, -(cur_lenz-3))*(-bot.relative_pos));
                }
            }
        }
    }

    void lower_action(set<Coordinate>& bot_filled) {
        for(auto& bot : botset) {
            if(bot.is_lower()) {
                if(bot.reached()) {
                    bot.halt();
                }else{
                    Coordinate next_pos = bot.cur_pos - dy;
                    int next_pos_int = Coordinate::ctoi(next_pos, resol);
                    if(field[next_pos_int]){
                        bot.void_(-dy);
                        field[next_pos_int] = false;
                    }else{
                        for(int adv = 1; adv <= MAX_SMOVE; adv++){
                            if(next_pos == bot.goal_pos || field[Coordinate::ctoi(next_pos - dy, resol)]) break;
                            next_pos -= dy;
                        }
                        bot.smove(next_pos - bot.cur_pos);
                        bot.cur_pos = next_pos;
                    }
                }
                bot_filled.insert(bot.cur_pos);
            }
        }
    }

    void upper_action(set<Coordinate>& bot_filled) {
        for(auto& bot : botset){
            if(!bot.is_lower()){
                if(bot.reached()) {
                    bot.halt();
                }else{
                    Coordinate next_pos = bot.cur_pos - dy;
                    if(bot_filled.count(next_pos)){
                        bot.halt();
                    }else{
                        for(int adv = 1; adv <= MAX_SMOVE; adv++){
                            if(next_pos == bot.goal_pos || bot_filled.count(next_pos - dy)) break;
                            next_pos -= dy;
                        }
                        bot.smove(next_pos - bot.cur_pos);
                        bot.cur_pos = next_pos;
                    }
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

    void come_back() {

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
        while(true) {
            do {
                boring();
            } while(erase_cubioid());
            if(all_end_check()) break;
            move();
            swap_upper_lower();
        }
        come_back();
    }

    void determine() {
        reverse();
    }

    inline int cur_time() {
        return clock_time;
    }
};

#endif
