//returns if adj describes a DAG
bool kahn(AdjacencyList& adj, queue<int>& noPreReq, vi& degree,
	  vi& sequence){
  while(!noPreReq.empty()){
    int u = noPreReq.front();
    noPreReq.pop();
    
    sequence.push_back(u);

    for(auto p: adj[u]){
      int v = p.first;
      degree[v]--;
      if (degree[v] < 0) { return false; }
      else if (degree[v] == 0) {
	noPreReq.push(v);
      }
      
    }
  }

  return true;
}
