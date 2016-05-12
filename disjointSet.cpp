#include <algorithm>
#include <vector>
#include <iostream>
#include <csdtio>
#include <cstdlib>

using namespace std;

class DisjointSet{

private:
  vector<size_t> parent, rank;

public:
  DisjointSet(size_t n){
    for (size_t i = 0; i < n; i++){
      parent.push_back(i);
      rank.push_back(0);
    }
  }

  size_t findSet(size_t i){
    if (parent[i] == i) { return i; }

    size_t ans = findSet(parent[i]);
    parent[i] = ans;
    return ans;
  }

  size_t unionSets(size_t x, size_t y){
    size_t sx = findSet(x);
    size_t sy = findSet(y);
    size_t superior = sx;
    size_t inferior = sy;
    if (rank[sy] > rank[sx]) { swap(superior,inferior); }

    if (sx != sy) {
      parent[inferior] = superior;
      if (rank[superior] == rank[inferior]) { rank[superior]++; }
    }

    return superior;
  }

};

//n nodes
//pq entries map (-weight) to (edge number)
//ith edge contains (u,v)
//returns tree weight and fills vector with treeEdges
double kruskal(int n, priority_queue<pair<double,size_t> >& pq,
	    vector<pair<size_t,size_t> >& edges,
	    vector<pair<size_t,size_t> >& treeEdges
	    ){
  double w = 0;
  DisjointSet ds(n);
  while(pq.size()){
    pair<double,size_t> p = pq.top();
    pq.pop();

    size_t e = p.second;
    pair<size_t,size_t> edge = edges[e];
    size_t u = edge.first;
    size_t v = edge.second;

    size_t uRoot = ds.findSet(u);
    size_t vRoot = ds.findSet(v);
    if (uRoot != vRoot){
      treeEdges.push_back(edge);
      ds.unionSets(uRoot,vRoot);
      w += -1 * p.first;
    }

  }

  return w;
}
