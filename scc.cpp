void tarjanRec(AdjacencyList& adj, int u,
	       int& group, vi& node2group, vi& node2lowgroup,
	       vector<bool>& onStack, stack<int>& s,
	       vi& rep){
  node2group[u] = group;
  node2lowgroup[u] = group;
  group++;

  s.push_back(u);
  onStack[u] = true;

  for(auto p: adj[u]){
    int v = p.first;
    if (node2group[v] == -1) {
      tarjanRec(adj,v,group,node2group,node2lowgroup,onStack,s,rep);
      node2lowgroup[u] = min(node2lowgroup[u],node2lowgroup[v]);
    }
    else if (onStack[v]) {
      node2lowgroup[u] = min(node2lowgroup[u],node2group[v]);
    }

  }

  if (node2group[u] == node2lowgroup[u]) {
    rep.push_back(u);

    int v = s.back();
    while(u != v){
      s.pop_back();
      onStack[v] = false;
      v = s.back();
    }

  }

}

//at end, rep contains one node for each scc
void tarjan(AdjacencyList& adj, vi& rep){
  int group = 0;
  
  vi node2group, node2lowgroup;
  stack<int> s;
  vector<bool> onStack;
  for(int u = 0; u < adj.size(); u++){
    node2group.push_back(-1);
    node2lowgroup.push_back(-1);
    onStack.push_back(false);
  }
  

  for(int u = 0; u < adj.size(); u++){
    if (node2group[u] == -1) {
      tarjanRec(adj,u,node2group,node2lowgroup,onStack,s,rep);
    }
  }

}
