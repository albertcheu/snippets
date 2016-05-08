
//|adj| = n
bool bellmanFord(int n, AdjacencyList& adj, int root, vi& node2root){
  vector<int> dist;
  vector<int> parent;
  for (auto u: adj){
    dist.push_back(INT_MAX);
    parent.push_back(-1);
  }

  dist[root] = 0;
  node2root[root] = root;

  for(int i = 1; i <= n-1; i++){
    for(int u = 0; u < n; u++){
      for(auto p: adj[u]){
	int v = p.first;
	int d = p.second;
	if (dist[v] > dist[u] + d) {
	  dist[v] = dist[u] + d;
	  parent[v] = u;
	  node2root[v] = root;
	}
      }
    }
  }

  for(int u = 0; u < n; u++){
    for(auto p: adj[u]){
      int v = p.first;
      int d = p.second;
      if (dist[v] > dist[u] + d) { return true; }
    }
  }  

  return false;
}

bool hasNegativeCycle(int n, AdjacencyList& adj) {
  //map node to the root of the search tree
  vi node2root;
  for (auto u: adj){ node2root.push_back(-1); }

  for(int u = 0; u < n; u++){
    if (node2root[u] == -1 && bellmanFord(n,adj,u,node2root))
      { return true; }
  }

  return false;
}
