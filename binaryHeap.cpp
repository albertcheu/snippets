#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <iostream>

//binary heap with change key

using namespace std;

template <typename T>
class MinHeap{
private:

  // the array to hold the keys in heap order
  vector<T> h;
  // map key to location in above array
  map<T, size_t> loc;

  void swapChild(size_t i, size_t child){
    loc[h[child]] = i;
    loc[h[i]] = child;
    swap(h[i],h[child]);
  }

  void pushDown(size_t i){
    size_t left = 2*i;
    size_t right = left+1;

    while (left < h.size()) {
      T cur = h[i];
      T leftChild = h[left];

      //there is no right child
      if (right >= h.size()) {
	//min-heap property violated
	if (leftChild < cur) {
	  swapChild(i,left);
	  i = left;
	}
	else { break; }
      }

      // there is a right child
      else {
	T rightChild = h[right];

	//min-heap property violated	
	if (leftChild < cur || rightChild < cur) {
	  if (leftChild < rightChild) {
	    swapChild(i,left);
	    i = left;
	  }
	  else {
	    swapChild(i,right);
	    i = right;
	  }
	}

	else { break; }
      }

      left = 2*i;
      right = 2*i + 1;
    
    }

  }


  void bubbleUp(size_t i){
    T key = h[i];
    while(i != 1 && h[i/2] > key){
      T parent = h[i/2];

      swap(h[i/2],h[i]);
    
      loc[parent] = i;
      loc[key] = i/2;

      i /= 2;
    }
  }


public:
  MinHeap() {
    h.push_back(T());
  }

  void print(){
    for(size_t i = 1; i < h.size()-1; i++){
      cout << h[i] << ' ';
    }
    cout << h.back() << endl;
  }

  void insert(T key){
    h.push_back(key);

    size_t i = h.size()-1;
    loc[key] = i;

    bubbleUp(i);
  }
  
  T extractMin(){
    T ret = h[1];

    size_t i = 1;
    m.erase(ret);

    T last = h.back();
    h[1] = last;
    loc[last] = i;
    h.pop_back();

    if (h.size() > 1) { pushDown(i); }

    return ret;
  }

  void changeKey(T oldKey, T newKey){
    if (loc.find(oldKey) == loc.end()) { return; }

    size_t i = loc[oldKey];
    if (oldKey != newKey){
      h[i] = newKey;
      loc.erase(oldKey);
      loc[newKey] = i;

      if (newKey > oldKey){ pushDown(i); }

      else { bubbleUp(i); }
    }

  }

  bool empty() { return h.size() == 1; }
  
};

class MaxHeap: public MinHeap{
public:
  MaxHeap() {
    h.push_back(T());
  }

  void insert(T key){
    MinHeap::insert(key*-1);
  }
  
  T extractMax(){
    return MinHeap::extractMin() * -1;
  }

}


int main(){
  MinHeap<int> h;

  h.insert(5);
  h.print(); 
  h.insert(10);
  h.print(); 
  h.insert(20);
  h.print(); 
  h.insert(12);
  h.print(); 
  h.insert(11);
  h.print(); 
  h.insert(2);
  h.print(); 

  h.changeKey(12,7);
  h.print();
  h.changeKey(2,12);
  h.print();

  h.extractMin();
  h.print();
}
