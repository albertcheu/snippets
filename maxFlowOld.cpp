#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <algorithm>
#include <climits>

#define ll long long
#define vi vector<int>
#define vll vector<ll>

//For node u, adj[u] is a map from neighbor v to {original capacity, residual}
//In short, adj[u][v] = Capacity(original, residual)
#define ListEntry map<int,Capacity>

using namespace std;

struct Capacity{
  ll original, residual;
  Capacity(): original(0), residual(0) {}
  Capacity(ll cap, bool forward) : residual(cap) {
    if (forward) { original = cap; }
    else { original = 0; }
  }
};

//if undirected, forward and backward are identical
//otherwise, there is no original capacity for backward flow
void initCapacity(ll cap, Capacity& uv, Capacity& vu, bool undirected){
  uv = Capacity(cap, true);
  vu = Capacity(cap, undirected);
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
      Capacity c = p.second;
      ll original = c.original;
      ll residual = c.residual;

      //cannot flow backwards if we haven't flowed forwards
      bool isBackward = (original == 0);
      bool haventForward = (adj[v][u].residual == adj[v][u].original);
      if (isBackward && haventForward) { continue; }

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
    ll residual = adj[u][v].residual;
    
    if (ans > residual) { ans = residual; }
    v = u;
  }

  //send the residual flow
  v = sink;
  while(v != source){
    int u = parent[v];
    //Cases of infinite capacity
    if (adj[u][v].original != LLONG_MAX) {
      //forwards
      adj[u][v].residual -= ans;
      //backwards
      adj[v][u].residual += ans;
    }

    v = u;
  }

  return ans;
}  

//Ford Fulkerson
//NB: in adj, if an edge capacity is "infinite", set it to be undirected
//NB: if there is a(n anti)parallel edge, insert an intermediary node
ll maxFlow(int source, int sink, vector<ListEntry>& adj) {
  ll ans = 0;

  while(true){
    vi parent;
    for(auto i: adj) { parent.push_back(INT_MAX); }
    parent[source] = -1;

    findAugPath(source,sink,adj,parent);
    //cout << "found path" << endl;

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
      Capacity c = p.second;

      //Is uv a forward edge that has some space left?
      if (!reachable[v] && c.original != 0 && c.residual != 0) {
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
