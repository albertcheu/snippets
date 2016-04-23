#include <algorithm>
#include <vector>
#include <queue>
#include <iostream>
#include <cstdio>
#include <cstdlib>     /* srand, rand */
#include <ctime>       /* time */

using namespace std;

template <typename T>

class SegmentTree{
private:
  struct Node{

    T val, lazy;
    size_t left, right;
    Node* parent, *leftChild, *rightChild;

    Node(size_t left, size_t right, Node* parent = 0)
      :left(left), right(right), val(T()),
       lazy(0), parent(parent), leftChild(0), rightChild(0)
    {}
  };

  size_t n;
  Node* root;
  vector<Node*> indexToNode;

  T queryRecursive(size_t qLeft, size_t qRight, Node* u){
    size_t left = u->left;
    size_t right = u->right;

    T delta = u->lazy;
    u->lazy = 0;

    Node* leftChild = u->leftChild;
    Node* rightChild = u->rightChild;
    if (leftChild) {
      leftChild->val += delta;
      leftChild->lazy += delta;
      rightChild->val += delta;
      rightChild->lazy += delta;
    }

    if (qLeft == left && qRight == right) {
      
      return u->val;
    }
    
    size_t mid = (left+right)/2;

    T ans = 0;
    if (qRight <= mid){
      ans = queryRecursive(qLeft,qRight,leftChild);
    }

    else if (qLeft >= mid){
      ans = queryRecursive(qLeft,qRight,rightChild);
    }

    else {
      T a = queryRecursive(qLeft,mid,leftChild);
      T b = queryRecursive(mid,qRight,rightChild);
      ans = ((a > b) ? a : b);
    }

    u->val = rightChild->val;
    if (leftChild->val > rightChild->val) { u->val = leftChild->val; }

    return ans;
  }

  void updateRange(size_t uLeft, size_t uRight, Node* u, T delta){
    size_t left = u->left;
    size_t right = u->right;
    Node* leftChild = u->leftChild;
    Node* rightChild = u->rightChild;

    if (uLeft == left && uRight == right) {
      u->val += delta;
      u->lazy += delta;
      return;
    }

    size_t mid = (left+right)/2;

    if (uRight <= mid){
      updateRange(uLeft,uRight,u->leftChild,delta);
    }

    else if (uLeft >= mid){
      updateRange(uLeft,uRight,u->rightChild,delta);
    }

    else {
      updateRange(uLeft,mid,u->leftChild,delta);
      updateRange(mid,uRight,u->rightChild,delta);
    }

    u->val = rightChild->val;
    if (leftChild->val > rightChild->val) { u->val = leftChild->val; }
  }

  void cleanup(Node* u){
    if (u->leftChild) {
      cleanup(u->leftChild);
      cleanup(u->rightChild);
    }
    delete u;
  }


public:

  SegmentTree(size_t n)
    :n(n)
  {
    queue<Node*> q;
    root = new Node(0,n);
    root->parent = root;
    q.push(root);

    for(size_t i = 0; i < n; i++){ indexToNode.push_back(0); }

    while(!q.empty()){
      Node* u = q.front();
      q.pop();

      size_t left = u->left;
      size_t right = u->right;

      if (left+1 != right) {
	size_t mid = (left+right)/2;

	Node* leftChild = new Node(left,mid,u);
	u->leftChild = leftChild;

	Node* rightChild = new Node(mid,right,u);
	u->rightChild = rightChild;

	q.push(leftChild);
	q.push(rightChild);
      }

      else { indexToNode[left] = u; }

    }

  }

  ~SegmentTree(){
    cleanup(root);
  }

  T query(size_t left, size_t right)
  { return queryRecursive(left,right,root); }

  void update(size_t left, size_t right, T delta)
  { updateRange(left,right,root,delta); }

  void update(size_t index, T newVal){
    Node* u = indexToNode[index];

    while(u->val < newVal) {
      u->val = newVal;
      u = u->parent;
    }
    
  }

  void debug(){
    for(size_t i = 0; i < n; i++){
      cout << indexToNode[i]->val << ' ';
    }
    cout << endl;

    queue<Node*> q;
    q.push(root);
    while(!q.empty()){
      Node * u = q.front();
      q.pop();
      if (u->leftChild) {
	q.push(u->leftChild);
	q.push(u->rightChild);
      }
      printf("(%lu,%lu,",u->left,u->right);
      cout << u->lazy << ") ";
    }
    cout << endl;
  }

};


int main(){
  vector<int> v;
  SegmentTree<int> st(10);
  srand(time(0));
  //int x[10] = {53,30,75,15,65,68,52,37,49,46};
  for(int i = 0; i < 10; i++){
    //st.update(i,x[i]);
    int r = rand()%100;
    st.update(i,r);
    v.push_back(r);
    cout << r << ' ';
  }
  cout << endl;
  //st.debug();

  /*
  st.update(2,3,24);
  st.update(7,8,-34);
  st.update(1,6,-20);
  st.update(0,8,-12);
  st.update(7,9,-41);
  */

  for(int i = 0; i < 5; i++){
    int ra = rand()%11;
    int rb = rand()%11;
    if (ra > rb) { swap(ra,rb); }
    if (ra == rb) {
      rb = ra+1;
    }

    int delta = rand()%50;
    if (rand()%2) { delta *= -1; }
    printf("adding %d to [%d,%d)\n",delta,ra,rb);
    for(int j = ra; j < rb; j++){
      v[j] += delta;
    }
    st.update(ra,rb,delta);
  }
  for(auto i: v) { cout << i << ' '; }
  cout << endl;

  //st.debug();

  for(int i = 0; i < 10; i++){
    int ra = rand()%11;
    int rb = rand()%11;

    if (ra > rb) { swap(ra,rb); }
    if (ra == rb) {
      rb = ra+1;
    }

    int ans = st.query(ra,rb);

    printf("max[%d,%d) = %d\n",ra,rb,ans);
  }

}
