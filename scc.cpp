
//Based on NYU codebook
void tarjanRec (AdjacencyList& adj, int i,
		vi& visited, vi& low, int& t,
		stack<int>& s,
		vi& sccNum, int& scc) {
  if (visited[i]) { return; }
  t++;
  visited[i] = low[i] = t;
  s.push(i);
  for(auto p: adj[i]){
    tarjanRec(adj,p.first,visited,low,t,s,sccNum,scc);
    low[i] = min(low[i],low[p.first]);
  }

  if (low[i] == visited[i]){
    while(true){
      int k = s.top();
      s.pop();
      sccNum[k] = scc;
      low[k] = INT_MAX;
      if (k == i) { break; }
    }
    scc++;
  }

}

//returns the number of connected components
//sccNum maps node to scc (the "group's number")
int tarjan(AdjacencyList& adj, vi& sccNum){
  size_t n = adj.size();

  int t = 0;
  int scc = 0;
  vi visited(n), low(n);
  stack<int> s;

  repeat(i,n) { tarjanRec(adj,i,visited,low,t,s,sccNum,scc); }

  return scc;
}
