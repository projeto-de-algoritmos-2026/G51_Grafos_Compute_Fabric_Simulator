#include "../include/UnionFind.hpp"

UnionFind::UnionFind(int n) {
    parent.resize(n);
    std::iota(parent.begin(), parent.end(), 0);
    rank.assign(n, 0);
}

int UnionFind::find(int i) {
    if (parent[i] == i)
        return i;
    return parent[i] = find(parent[i]); // Compressão de caminho
}

bool UnionFind::unite(int i, int j) {
    int rootI = find(i);
    int rootJ = find(j);

    if (rootI != rootJ) {
        if (rank[rootI] < rank[rootJ])
            std::swap(rootI, rootJ);
        parent[rootJ] = rootI;
        if (rank[rootI] == rank[rootJ])
            rank[rootI]++;
        return true;
    }
    return false;
}