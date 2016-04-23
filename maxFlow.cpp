#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <algorithm>
#include <climits>

#define ll long long
#define vi vector<int>
#define vll vector<ll>

//For node u, adj[u] is a map from neighbor v to {capacity, flow}
//In short, adj[u][v] = Edge(capacity, flow)
#define ListEntry map<int,Edge>

using namespace std;

struct Edge{
  ll capacity, flow;
  Edge(): capacity(0), flow(0) {}
  Edge(ll cap, bool forward) : flow(0) {
    if (forward) { capacity = cap; }
    else { capacity = 0; }
  }
};

//if undirected, forward and backward are identical
//otherwise, there is no original capacity for backward flow
void initEdge(ll cap, Edge& uv, Edge& vu, bool undirected){
  uv = Edge(cap, true);
  vu = Edge(cap, undirected);
}

//Edmonds-Karp (BFS)
void findAugPath(int source, int sink,
		 vector<ListEntry>& adj,
		 vi& parent){
  queue<int> q;
  q.push(source);

  while(!q.empty()){
    int u = q.front();
    q.pop();

    for(auto p: adj[u]){
      int v = p.first;
      Edge e = p.second;
      ll capacity = e.capacity;
      ll flow = e.flow;

      //cannot flow backwards if we haven't flowed forwards
      bool isBackward = (capacity == 0);
      bool haventForward = (adj[v][u].flow == 0);
      if (isBackward && haventForward) { continue; }

      ll residual = capacity - flow;

      //if we haven't seen this node before and we can send flow
      if (parent[v] == INT_MAX && residual > 0) {
	parent[v] = u;

	if (v == sink) { break; }
	else { q.push(v); }
      }
    }

    if (parent[sink] != INT_MAX) { break; }
  }
  /*
  for(int u = 0; u < adj.size(); u++){
    printf("(%d %d) ",u,parent[u]);
  }
  printf("\n");
  */
}


ll sendFlow(int source, int sink,
	    vector<ListEntry>& adj,
	    vi& parent){
  //no augmenting path
  if (parent[sink] == INT_MAX) {
    //cout << "no path" << endl;
    return 0;
  }

  //get min residual
  ll ans = LLONG_MAX;
  int v = sink;
  while(v != source){
    int u = parent[v];
    ll capacity = adj[u][v].capacity;
    
    ll residual = LLONG_MAX;
    if (capacity != LLONG_MAX) { residual = capacity - adj[u][v].flow; }
    
    if (ans > residual) { ans = residual; }
    v = u;
  }

  //send the residual flow
  v = sink;
  while(v != source){
    int u = parent[v];
    //forwards
    adj[u][v].flow += ans;
    //backwards
    adj[v][u].flow -= ans;

    v = u;
  }

  return ans;
}  

//Ford Fulkerson
//NB: if there is a(n anti)parallel edge, insert an intermediary node
ll maxFlow(int source, int sink, vector<ListEntry>& adj) {
  ll ans = 0;

  while(true){
    vi parent;
    for(auto i: adj) { parent.push_back(INT_MAX); }
    parent[source] = -1;

    findAugPath(source,sink,adj,parent);
    //cout << "ended search for sink" << endl;

    ll flow = sendFlow(source,sink,adj,parent);
    //cout << "sent flow " << flow << endl;

    ans += flow;
    if (flow == 0) { break; }
  }

  return ans;
}

//BFS to flag nodes reachable from the source
void findCC(int source, vector<ListEntry>& adj, vector<bool>& reachable){
  queue<int> q;
  q.push(source);
  reachable[source] = true;

  while(!q.empty()){
    int u = q.front();
    q.pop();

    for(auto p: adj[u]) {
      int v = p.first;
      Edge e = p.second;

      //Is uv a forward edge that has some space left?
      if (!reachable[v] && e.original != 0 && e.capacity > e.flow) {
	reachable[v] = true;
	q.push(v);
      }
    }

  }

}


ll minCut(int source, int sink, vector<ListEntry>& adj,
	  vector<pair<int,int> >& cut) {

  ll ans = maxFlow(source, sink, adj);

  //reachable[u] iff u is reachable from source
  vector<bool> reachable;

  for(int i = 0; i < adj.size(); i++){ reachable.push_back(false);  }
  
  findCC(source,adj,reachable);

  for(int u = 0; u < adj.size(); u++){
    // reachable to unreachable, not other way around
    if (!reachable[u]) { continue; }

    for(auto p: adj[u]) {
      int v = p.first;
      
      if (!reachable[v]) { cut.push_back({u,v}); }
    }
  }

  return ans;
}

int main(){

}
