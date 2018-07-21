#include <bits/stdc++.h>
#define ll long long
#define INF 1000000005
#define MOD 1000000007
#define EPS 1e-10
#define rep(i,n) for(int i=0;i<(int)(n);++i)
#define rrep(i,n) for(int i=(int)(n)-1;i>=0;--i)
#define srep(i,s,t) for(int i=(int)(s);i<(int)(t);++i)
#define each(a,b) for(auto& (a): (b))
#define all(v) (v).begin(),(v).end()
#define len(v) (int)(v).size()
#define zip(v) sort(all(v)),v.erase(unique(all(v)),v.end())
#define cmx(x,y) x=max(x,y)
#define cmn(x,y) x=min(x,y)
#define fi first
#define se second
#define pb push_back
#define show(x) cout<<#x<<" = "<<(x)<<endl
#define spair(p) cout<<#p<<": "<<p.fi<<" "<<p.se<<endl
#define sar(a,n) cout<<#a<<":";rep(pachico,n)cout<<" "<<a[pachico];cout<<endl
#define svec(v) cout<<#v<<":";rep(pachico,v.size())cout<<" "<<v[pachico];cout<<endl
#define svecp(v) cout<<#v<<":";each(pachico,v)cout<<" {"<<pachico.first<<":"<<pachico.second<<"}";cout<<endl
#define sset(s) cout<<#s<<":";each(pachico,s)cout<<" "<<pachico;cout<<endl
#define smap(m) cout<<#m<<":";each(pachico,m)cout<<" {"<<pachico.first<<":"<<pachico.second<<"}";cout<<endl

using namespace std;

typedef pair<int,int> P;
typedef pair<ll,ll> pll;
typedef vector<int> vi;
typedef vector<vi> vvi;
typedef vector<ll> vl;
typedef vector<vl> vvl;
typedef vector<double> vd;
typedef vector<P> vp;
typedef vector<string> vs;

const int MAX_N = 100005;

class UnionFind;
class Coordinate;
class Bot;
class Game;


class UnionFind {
private:
    int node_size;
    vector<int> par, rank, size;

public:
    UnionFind(){}

    UnionFind(int sz) : node_size(sz), par(node_size), rank(node_size, 0), size(node_size, 1){
        iota(par.begin(), par.end(), 0);
    }

    int find(int x) {
        if(par[x] == x) {
            return x;
        }else{
            return par[x] = find(par[x]);
        }
    }

    void unite(int x, int y) {
        x = find(x), y = find(y);
        if(x == y) return;
        if(rank[x] < rank[y]) {
            swap(x,y);
        }
        par[y] = x, size[x] += size[y];
        if(rank[x] == rank[y]) {
            rank[x]++;
        }
    }
    int query(int x) {
        x = find(x);
        return size[x];
    }
    bool same(int x, int y) {
        return find(x) == find(y);
    }
};


class Coordinate {
public:
    int x, y, z;
    Coordinate(){};
    Coordinate(int x_, int y_, int z_) : x(x_), y(y_), z(z_){};

    Coordinate operator+ (const Coordinate& another) const {
        return Coordinate(this->x+another.x, this->y+another.y, this->z+another.z);
    }

    bool operator== (const Coordinate& another) const {
        return (x == another.x) && (y == another.y) && (z == another.z);
    }

    static int ctoi(Coordinate original, int R){
        return original.x*R*R + original.y*R + original.z;
    }

    static Coordinate itoc(int original, int R){
        return Coordinate(original/R/R, (original%R)/R, original%R);
    }
};


class Bot {
public:
    string command[12] = {"Halt", "Wait", "Flip", "SMove", "LMove", "Fission", "Fill", "Void", "FusionP", "FusionS", "GFill", "GVoid"};
    vector<vector<int> > hist_command;
    int identifier, parent, clock_time;
    Coordinate cur_pos, target;
    set<int> child;
    bool upper;

    Bot(){}

    Bot(int id_, int par_) : identifier(id_), parent(par_){}

    Bot(int id_, set<int>& child_, Coordinate cur_pos_, int time_) : identifier(id_), parent(-1),
        clock_time(time_), cur_pos(cur_pos_), child(child_){}

    inline bool is_lower() {
        return upper;
    }

    inline bool reached() {
        return (cur_pos == target);
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

    void smove(Coordinate& dir) {

    }

    void lmove() {

    }

    void fission() {

    }

    void fill(Coordinate& dir) {

    }

    void void_(Coordinate& dir) {

    }

    void fusionp(Coordinate& dir) {

    }

    void fusions(Coordinate& dir) {

    }

    void gfill() {

    }

    void gvoid() {

    }
};


class Game {
public:
    static const int MAX_BORING_SIZE = 10;
    const int BLOCK_SIZE = 28;
    const int MAX_SEED = 40;
    const Coordinate INIT_POS = Coordinate(0, 0, 0);
    vector<Bot> botset;

private:
    int resol, cube, voxel_count, bot_count, clock_time;
    bool* field;
    ll energy_sum;
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

    bool boring() {
        alloc_target();
        lower_action();
        upper_action();
    }

    bool erase() {

    }

    void lower_action() {
        for(auto& bot : botset) {
            if(bot.is_lower()) {
                if(bot.reached()) {
                    bot.halt();
                }else{
                    Coordinate next_pos = bot.cur_pos + Coordinate(0, 0, -1);
                    int next_pos_int = ctoi(next_pos, resol);
                    if(field[next_pos_int]){
                        bot.void_(Coordinate(0, 0, -1));
                        field[next_pos_int] = false;
                    }else{
                        bot.
                    }
                }
            }
        }
    }

    void upper_action() {
        for(auto& bot : botset){
            if(!bot.is_lower()){

            }
        }
    }

    void upper_move() {

    }

    void lower_move() {

    }

    bool end_check() {

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
                voxel_component.unite(Coordinate::ctoi(Coordinate(x, 0, z), resol),
                    Coordinate::ctoi(Coordinate(x+1, 0, z), resol));
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
        field[x_][y_][z_] = true;
    }

    void play() {
        pre_conditioning();
        while(true){
            do {
                while(boring()){};
            } while(erase());
            if(end_check()) break;
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

void input(Game& game) {
    int n, R;
    cin >> n >> R;
    game.build(R);
    rep(i,n) {
        int x, y, z;
        cin >> x >> y >> z;
        game.voxel_set(x, y, z);
    }
}

void output(Game& game) {
    char output_file_name[] = "./output.txt";
    ofstream fout(output_file_name, ios::out);
    vector<vector<vector<int> > > command_set(game.cur_time(), vector<vector<int> >(game.MAX_SEED));
    for(auto& bot : game.botset) {
        for(auto& command : bot.hist_command) {
            int tm = command.back();
            command.pop_back();
            command_set[tm][bot.identifier] = command;
        }
    }
    for(auto& commands : command_set) {
        for(auto& command : commands) {
            for(auto& num : command) {
                fout << num << " ";
            }
            fout << "\n";
        }
    }
}

void solve() {
    Game game;
    input(game);
    game.play();
    game.determine();
    output(game);
}

int main() {
    cin.tie(0);
    ios::sync_with_stdio(false);
    solve();
    return 0;
}
