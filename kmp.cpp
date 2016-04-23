/*Knuth-Morris-Pratt*/
//Fast DFA-based string matching

//"border" = prefix which is also a proper suffix
//len[i] = length of longest border of pattern up to but not including ith char
//len has size pattern.size+1
void kmp_build(const string& pattern, vi& len){

  for (int i = 2; i <= pattern.size(); i++) {
    int j = len[i-1];

    while (true) {
      //if this character matches (the biggest border)+1,
      //we don't have to look any further
      if (pattern[j] == pattern[i-1]) { len[i] = j+1; break;}

      //if the biggest border is empty, this char can't be a prefix
      else if (j == 0) { break; }

      //shift what we consider to be the biggest border
      else { j = len[j]; }
    }

  }

}

//here, len has not been filled yet
int shortestPeriod(const string& pattern, vi& len){
  int size = pattern.size();
  int ans = size;

  for (int i = 2; i <= size; i++) {
    int j = len[i-1];

    while (true) {
      bool a = (i != size);
      bool b = (size%(size-j-1) == 0);

      //if this character matches (the biggest border)+1,
      //we don't have to look any further
      if (pattern[j] == pattern[i-1] && (a || b)) {
	len[i] = j+1;

	//we're at the last spot & it satisfies the x^k condition
	if (!a && b) { ans = size-j-1; }

	break;
      }

      //if the biggest border is empty, this char can't be a prefix
      else if (j == 0) { break; }
      //shift what we consider to be the biggest border
      else { j = len[j]; }
    }

  }

  return ans;
}

//returns state (match if equal to pattern size)
int kmp_compare(const string& text, const string& pattern, const vi& len){
  int state = 0;

  for (int i = 0; i < text.size(); i++) {

    while (true) {
      //match = move along DFA
      if (text[i] == pattern[state]) { state++; break; }

      //restarted DFA
      else if (state == 0) { break; }

      //smart restart
      state = len[state];
    }
    
    //in the accept state
    if (state == pattern.size()) { break; }

  }

  return state;
}

//assume pattern ends with a unique token (not present in pattern or text)
//fills vector with indices in text that start instances of the pattern
//returns number of occurrences (vector's size)
int fillOccurrences(const string& text, const string& pattern, const vi& len,
		    vi& occurrences){
  int counter = 0;

  int trueSize = pattern.size()-1;
  int state = 0;

  for (int i = 0; i < text.size(); i++) {

    while (true) {
      //match = move along DFA
      if (text[i] == pattern[state]) { state++; break; }

      //restarted DFA
      else if (state == 0) { break; }

      //smart restart
      state = len[state];
    }
    
    if (state == trueSize) {
      counter++;
      occurrences.push_back(i-trueSize+1);
    }

  }

  return counter;
}
