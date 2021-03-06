
//map [0-9] to ['0'-'9']
char num2char(int x) { return x + 48; }

//map [0,25] to [A-Z]
char num2upper(int x) { return x + 65; }

//map [0,25] to [a-z]
char num2lower(int x) { return x + 97; }

//map ['0'-'9'] to [0-9]
int char2num(char c) { return c - 48; }

//map [A-Z] to [0,25]
int upper2num(char c) { return c - 65; }

//map [a-z] to [0,25]
int lower2num(char c) { return c - 97; }

//gosper's
void snoob(ull& x){
  ull y = x & -x;
  ull c = x + y;
  x = (((x ^ c) >> 2) / y) | c;
}

ll gcd(ll a, ll b){
  while(a != b){
    if (a > b) { swap(a,b); }
    b -= a;
  }
  return a;
}


ll lcm(ll a, ll b, ll g){
  ll x = a / g;
  ll y = b / g;
  //x and y must be coprime
  return x*y*g;
}

//Fenwick trees: dynamic subarray sum or prefix max
//ONE-BASED indexing
int sum(vi& v, int i) // Returns the sum from index 1 to i
{
  int sum = 0;
  while(i > 0) {
    sum += v[i];
    i -= i & -i;
  }
  return sum;
}
 
void add(vi& v, int i, int k) // Adds k to element with index i
{
  while (i < SIZE) {
    v[i] += k;//v[i] = max(v[i],k)
    i += i & -i;
  }
}

//Sieve of Eratosthenes
//find all primes less than r
void getPrimes(int r, vi& primes, set<int>& primeSet){
  vector<bool> table;
  for(int i = 0; i <= r; i++){ table.push_back(true); }
  table[0] = table[1] = false;

  int prime = 2;
  while(true){
    //if (prime < 200) { cout << prime << ' '; }
    primes.push_back(prime);
    primeSet.insert(prime);

    //sieve
    for(int i = prime; i < table.size(); i+=prime){
      table[i] = false;
    }

    //find next prime
    int start = prime+1;
    prime = -1;
    for(int i = start; i < table.size(); i++){
      if (table[i]) { prime = i; break; }
    }
    if (prime == -1) { break; }
  }

  //cout << endl;
}

//Repeated squaring
//(a^e) = (a^(e/2))^2 * a^(e%2)
ull modexp(ull a, ull e, ull p){
  if (e == 0) { return 1; }
  if (e == 1) { return a%p; }

  ull recur = modexp(a,e/2,p);
  ull x = (e%2)?(a%p):1;
  return ( ((recur*recur)%p) * x) % p;
} 

//Adapted from https://en.wikipedia.org/wiki/Levenshtein_distance
int editDistance(const string& s, const string& t){
  // degenerate cases
  if (s == t) return 0;
  if (s.size() == 0) return t.size();
  if (t.size() == 0) return s.size();

  // create two work vectors of integer distances
  vi prev(t.size()+1);
  vi cur(t.size()+1);

  // initialize previous row of distances
  // this row is A[0][i]: edit distance for an empty s
  // the distance is just the number of characters to delete from t
  for (int i = 0; i < prev.size(); i++) { prev[i] = i; }

  for (int i = 0; i < s.size(); i++) {
    // calculate current row distances from the previous row

    // first element is A[i+1][0]
    //   edit distance is delete (i+1) chars from s to match empty t
    cur[0] = i + 1;

    // use formula to fill in the rest of the row
    for (int j = 1; j < cur.size(); j++) {
      int replaceCost = prev[j-1] + ((s[i] == t[j-1]) ? 0 : 1);
      int dropIth = prev[j] + 1;
      int dropJth = cur[j-1] + 1;

      cur[j] = min(replaceCost, min(dropJth, dropIth));

    }

    swap(prev, cur);
  }

  return prev.back();
}

//calculate the nth number mod p in a fibonacci sequence:  fib(n)%p
int fastFib(int n,int p) {
  long long a[2][2]={{1,1},{1,0}};
  long long b[2][2]={{1,0},{0,1}};
  long long c[2][2];
  while(n) {

    //multiply matrix a and b
    if (n%2) {
      for(int i=0;i<2;i++)
	for(int j=0;j<2;j++)
	  c[i][j]=0;
      for(int i=0;i<2;i++)
	for(int j=0;j<2;j++)
	  for(int k=0;k<2;k++)
	    c[i][j]=(c[i][j]+a[i][k]*b[k][j])%p;
      for(int i=0;i<2;i++)
	for(int j=0;j<2;j++)
	  b[i][j]=c[i][j];
    }

    /**multiply matrix a and a*/
    for(int i=0;i<2;i++)
      for(int j=0;j<2;j++)
	c[i][j]=0;
    for(int i=0;i<2;i++)
      for(int j=0;j<2;j++)
	for(int k=0;k<2;k++)
	  c[i][j]=(c[i][j]+a[i][k]*a[k][j])%p;
    for(int i=0;i<2;i++)
      for(int j=0;j<2;j++)
	a[i][j]=c[i][j];
    n>>=1;
  }

  return (int)b[0][0];
}
