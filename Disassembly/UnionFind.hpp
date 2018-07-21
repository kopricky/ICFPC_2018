#ifndef UNIONFIND_HPP
#define UNIONFIND_HPP

#include <vector>
#include <algorithm>

using namespace std;

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

#endif
