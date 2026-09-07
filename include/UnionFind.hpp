#ifndef UNIONFIND_HPP
#define UNIONFIND_HPP

#include <vector>
#include <numeric>

// Estrutura Disjoint Set Union (DSU) com compressão de caminho e união por rank
class UnionFind {
private:
    std::vector<int> parent;
    std::vector<int> rank;

public:
    explicit UnionFind(int n);

    int find(int i);
    bool unite(int i, int j);
};

#endif // UNIONFIND_HPP