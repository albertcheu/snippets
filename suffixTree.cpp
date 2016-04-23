/*
  Implements linear-time construction of (generalized) suffix tree
  Sample input: "abcd$xyz#", where $ and # are delimeters
  Delimeters determined by isDelim

  Primary methods:

  occurrences(pattern):
    returns how many times pattern occurred across all input strings

  longestRepeat(vector of strings, k)
    Let Q = substrings that appear multiple times and in >k input strings
    method fills vector with longest such Q
    returns length of such strings
*/

#include <map>
#include <set>
#include <stack>
#include <string>
#include <limits>
#include <algorithm>
#include <iostream>
#include <sstream>

using namespace std;


//text has characters in range [LOWER,UPPER]
const char LOWER = 'a';
const char UPPER = 'z';

char ithDelim(int i){
  char ans = 0;

  if (i < LOWER-1) { ans = (char)(i+1); }
  else { ans = (char) (UPPER+1 + (i+1 - LOWER)); }

  //cerr << (int)ans << endl;

  return ans;
}

struct SuffixTree {
  //MUST DEFINE DELIMITERS OF GENERALIZED SUFFIX TREE
  bool isDelim(char c){
    return (c < LOWER) || (c > UPPER);
  }

  static const int INF = 1<<30;
  int last;
  string str;

  struct Node;
  struct Edge {
    int a,b;
    Node *begin,*end;
    SuffixTree* tree;
    Edge(int a, int b, Node *end, SuffixTree* tree)
      : a(a), b(b), begin(NULL), end(end), tree(tree) {}
    char getFirst() const { return tree->str[a]; }
    int size() const { return min(tree->last,b)-a+1; }
    friend ostream& operator<<(ostream& out, Edge& e);
  };

  struct Node {
    Edge* inEdge;
    map<char,Edge*> edges;
    Node* suffix;
    int a,b,leavesBelow,distinctDelims;
    SuffixTree* tree;
    Node(SuffixTree* tree)
      : tree(tree), leavesBelow(0), distinctDelims(0), inEdge(NULL) {}

    void add(Edge* e) { edges[e->getFirst()] = e; }
    Edge* operator[](char c) { return edges[c]; }
    int numChildren() { return edges.size(); }
    friend ostream& operator<<(ostream& out, Node&);
  };
    
  Node *root, *sentinel;

  Node* getRoot() { return root; }
  const string& getStr() const { return str; }
  
  void setPrefix(Node* n, Edge* e, int len) {
    map<char,Edge*>::iterator it = n->edges.begin();
    for (; it != n->edges.end(); ++it) {
      Edge *edge = it->second;
      setPrefix(edge->end,edge,len+min(edge->b,last)-edge->a+1);
    }

    if (e) {
      n->b = min(e->b,last);
      n->a = n->b - len + 1;
    }

    else {
      n->a = 0;
      n->b = -1;
    }
  }

  Node* testAndSplit(Node* s, int k, int p, char c) {
    if (k > p) {
      return s == sentinel ? NULL : (*s)[c] == NULL ? s : NULL;
    }

    Edge* e = (*s)[str[k]];
    if (c == str[e->a+p-k+1]) return NULL;
    Node* r = new Node(this);
    Edge* re = new Edge(e->a+p-k+1,e->b,e->end,this);
    r->add(re);
    Edge* se = new Edge(e->a,e->a+p-k,r,this);
    s->add(se);
    return r;
  }

  Node* canonize(Node* s, int& k, int p) {
    if (p < k) { return s; }

    if (s == sentinel) {
      s = root;
      ++k;
      if (p < k) { return s; }
    }

    Edge* e = (*s)[str[k]];
    while (e->b - e->a <= p - k) {
      k = k + e->b - e->a + 1;
      s = e->end;
      if (k <= p) e = (*s)[str[k]];
    }

    return s;
  }

  Node* update(Node* s, int& k, int i) {
    Node *oldr = root, *r = testAndSplit(s,k,i-1,str[i]);
    while (r) {
      Node* rp = new Node(this);
      Edge* e = new Edge(i,INF,rp,this);
      r->add(e);
      if (oldr != root) { oldr->suffix = r; }
      oldr = r;
      s = canonize(s->suffix,k,i-1);
      r = testAndSplit(s,k,i-1,str[i]);	    
    }

    if (oldr != root) { oldr->suffix = s; }

    return s;
  }

  void buildTree() {
    root = new Node(this);
    sentinel = new Node(this);
    root->suffix = sentinel;
    Node* s = root;
    int k = 0;
    last = -1;
    for (int i = 0; i < (int)str.size(); ++i) {
      ++last;
      s = update(s,k,i);
      s = canonize(s,k,i);
    }
  }

  int fix(int x) const { return x == INF ? last : x; }

  friend ostream& operator<<(ostream& out, SuffixTree& t);

  ostream& prettyFormat(ostream& out, Node* n, int tab) const {
    out << string(tab,' ') << "\"" << *n << "\"" << n->distinctDelims << endl;
    map<char,Edge*>::iterator it = n->edges.begin();

    for (; it != n->edges.end(); ++it) {
      Edge* e = it->second;

      out << string(tab,' ') << it->first << " : "
	   << *e << " = " << e->a << ',' << fix(e->b) << ',' << e->size() << endl;

      prettyFormat(out,e->end,tab+2);
    }

    return out;
  }

  //number of leaves in subtree
  //number of distinct delimiters
  int postprocess(Node* n, set<char>& delims) {
    for(auto p: n->edges){
      //assign parent pointers
      Edge* e = p.second;
      Node* child = e->end;

      child->inEdge = e;
      e->begin = n;

      //count number of leaves in subtree
      set<char> childDelims;
      n->leavesBelow += postprocess(child, childDelims);
      for(auto c: childDelims) { delims.insert(c); }
      
    }

    //leaf
    if (n->numChildren() == 0) {
      n->leavesBelow = 1;
      
      Edge* inEdge = n->inEdge;

      for(int i = 0; i < inEdge->size(); i++){
	if (isDelim(str[inEdge->a + i])) {
	  inEdge->b = inEdge->a + i;
	  n->b = inEdge->b;
	  break;
	}
      }

      int index = inEdge->a + inEdge->size() - 1;
      delims.insert(str[index]);
    }

    n->distinctDelims = delims.size();

    return n->leavesBelow;
  }

  SuffixTree(const string& str) : str(str) {
    buildTree();
    setPrefix(root,NULL,0);
    set<char> delims;
    postprocess(root,delims);
  }

  void destroyNode(Node* n){
    for(auto p: n->edges){
      Edge* e = p.second;
      destroyNode(e->end);
      delete e;
    }

    delete n;
  }

  ~SuffixTree(){
    destroyNode(root);
    delete sentinel;
  }

  int occurrences(const string& s, int index, Node* n){
    //cerr << "Called occurrences()" << endl;
    if (n->numChildren() == 0) {
      //cerr << "No children" << endl;
      return 0;
    }
    if (n->edges.find(s[index]) == n->edges.end()) {
      //cerr << "Cannot find " << s[index] << endl;
      return 0;
    }

    Edge* e = n->edges[s[index]];

    for(int i = e->a; i < e->a+e->size(); i++){

      if (str[i] == s[index]) {
	index++;
	if (index == s.size()) { break; }
      }

      else { return 0; }
    }

    if (index == s.size()) { return max(1,e->end->leavesBelow); }

    return occurrences(s,index,e->end);
  }

  int occurrences(const string& s){
    return occurrences(s,0,root);
  }
  
  void longestRepeat(Node* n, int size, int& length,
		     vector<Node*>& ptrs, int threshold){
    if (n != root
	//repeats iff there are multiple leaves in the subtree
	&& n->leavesBelow > 1
	//longest
	&& size >= length
	//check if the number of input strings is above the specified threshold
	&& n->distinctDelims > threshold
	) {
      if (size > length) {
	length = size;
	ptrs.clear();
      }
      ptrs.push_back(n);
      
    }

    for(auto p: n->edges){
      Edge* e = p.second;
      longestRepeat(e->end, size+e->size(), length, ptrs, threshold);
    }

  }

  int longestRepeat(vector<string>& longest, int threshold=0){
    int length = 0;
    vector<Node*> ptrs;

    longestRepeat(root, 0, length, ptrs, threshold);

    //reconstruct by walking up to root
    for (auto p: ptrs) {
      Node* ptr = p;
      string cur = "";

      stack<pair<int,int> > order;
      while(ptr != root) {
	order.push({ptr->inEdge->a, ptr->inEdge->b});
	ptr = ptr->inEdge->begin;
      }
      while(!order.empty()){
	pair<int,int> p = order.top();
	order.pop();
	for(int i = p.first; i <= p.second; i++){
	  cur += str[i];
	}
      }

      longest.push_back(cur);
    }

    return length;
  }
  
};

ostream& operator<<(ostream& out, SuffixTree::Edge& e)
{ return out << e.tree->str.substr(e.a,e.size()); }

ostream& operator<<(ostream& out, SuffixTree::Node& n)
{ return out << n.tree->str.substr(n.a,min(n.b,n.tree->last)-n.a+1); }	

ostream& operator<<(ostream& out, SuffixTree& t)
{ return t.prettyFormat(out, t.root, 0); }


int main(int argc, char** argv) {
  SuffixTree t(argv[1]);
  cout << t << endl;

  /*
  for(int i = 2; i < argc; i++){
    printf("%s occurs %d times\n", argv[i], t.occurrences((string)argv[i]));
  }
  */


  vector<string> longest;
  int x = t.longestRepeat(longest,1);
  cout << "The longest common substrings:" << endl;
  for(auto s: longest) {
    cout << '\t' << s << endl;
  }


  return 0;
}
