//Adapted from sites.google.com/site/stevenhalim/home/material
//or cpbook.net
#include <cmath>
#include <vector>
#include <stack>
#include <algorithm>
#include <iostream>
#include <cstdio>

#define COLLINEAR 0
#define LEFT 1
#define RIGHT -1
#define polygon vector<point>

using namespace std;
const double EPS = 0.000000001;

bool equal(double a, double b) { return fabs(a-b) < EPS; }

struct point;
struct line;
struct vec;

// use this whenever possible
//struct point { int x, y
struct point {
  double x,y;
  point();
  point(double _x, double _y);
  point(pair<double,double> p);
  bool operator == (const point& o);
  bool operator< (const point& other);
  vec operator- (const point& other);
  point operator+ (const vec& v);
};

struct line {
  double a, b, c;
  line (const point& p1, const point& p2);
  bool equivalent(const line& other) const;
  bool operator== (const line& other) const;
  //are they parallel
  bool parallel(const line& other) const;
  bool operator|| (const line& other) const;
};

//struct vec { int dx, dy;
struct vec {
  double dx, dy, mag;
  vec(double x, double y);
  vec(const point& from, const point& to); // convert 2 points to vector
  vec(pair<double,double> p);
  vec normalize();
  vec operator+ (const vec& other);
  //scale
  vec operator* (double s);
  //dot product
  double operator* (const vec& other);
};

point::point() :x(0), y(0) {}
point::point(double _x, double _y)
  :x(_x), y(_y)
{}
point::point(pair<double,double> p)
  :x(p.first), y(p.second)
{}
bool point::operator == (const point& o)
{ return equal(x,o.x) && equal(y,o.y); }
bool point::operator < (const point& other) {
  if (fabs(x - other.x) > EPS) return x < other.x;
  return y < other.y;
}
vec point::operator- (const point& other){ return vec(other,*this); }
point point::operator+ (const vec& v){ return point(x+v.dx,y+v.dy); }
double distance(const point& p1, const point& p2) {
  return hypot(p1.x - p2.x, p1.y - p2.y);
}

line::line (const point& p1, const point& p2) {
  // vertical line is handled nicely below
  //if (p1.x == p2.x) {
  if (equal(p1.x, p2.x)) { 
    a = 1.0;
    b = 0.0;
    c = -p1.x;
  }

  else {
    a = -(double)(p1.y - p2.y) / (p1.x - p2.x);
    b = 1.0;
    c = -(double)(a * p1.x) - (b * p1.y);
  }
}

bool line::parallel(const line& other) const {
  // check coefficient a + b
  return (fabs(a-other.a) < EPS) && (fabs(b-other.b) < EPS);
}

bool line::operator|| (const line& other) const {
  return parallel(other);
}

bool line::equivalent(const line& other) const {
  return parallel(other) && (fabs(c - other.c) < EPS);
}

bool line::operator== (const line& other) const {
  return equivalent(other);
}

bool intersect(const line& l1, const line& l2, point& p) {
  // all points same
  if (l1 == l2) { return false; }
  // no intersection
  if (l1 || l2) { return false; }

  // solve system of 2 linear algebraic equations with 2 unknowns
  p.x = (double)(l2.b * l1.c - l1.b * l2.c) / (l2.a * l1.b - l1.a * l2.b);

  // test for vertical line (avoid div by zero)
  if (fabs(l1.b) > EPS) { p.y = - (l1.a * p.x + l1.c) / l1.b; }
  else { p.y = - (l2.a * p.x + l2.c) / l2.b; }

  return true;
}


vec::vec(double deltaX, double deltaY)
  : dx(deltaX), dy(deltaY)
{
  double magX = fabs(dx);
  double magY = fabs(dy);
  if (magX > EPS && magY > EPS) { mag = hypot(magX, magY); }
  else if (magX < EPS) { mag = magY; }
  else if (magY < EPS) { mag = magX; }
  else { mag = 0; }
}

vec::vec(const point& from, const point& to)
  :vec(to.x - from.x, to.y - from.y)
{}
vec::vec(pair<double,double> p)
  :vec(p.first, p.second)
{}
vec vec::normalize() { return vec(dx / mag, dy / mag); }
vec vec::operator+ (const vec & other){
  return vec(dx + other.dx, dy + other.dy);
}
vec vec::operator* (double s){ return vec(s * dx, s * dy); }
double vec::operator* (const vec & other){
  return dx * other.dx + dy * other.dy;
}

//square of triangle area
/*
int area2(point a, point b, point c) {
  return a.x * b.y - a.y * b.x + b.x * c.y - b.y * c.x + c.x * a.y - c.y * a.x;
}
*/

// square of distance between 2 points
int dist2(point a, point b) {
  int dx = a.x - b.x, dy = a.y - b.y; return dx * dx + dy * dy;
}

int turn(const point& a, const point& b, const point& c) {
  double result = (c.x - b.x) * (a.y - b.y) - (c.y - b.y) * (a.x - b.x);

  if (result < 0) return RIGHT; // a->B->C is a right turn
  if (result > 0) return LEFT; // a->B->C is a left turn

  return COLLINEAR; // a->B->C is a straight line, i.e. a, B, C are collinear
}

//important angle-sorting functor
class LeftTurnCmp{
private:
  point pivot;
public:
  LeftTurnCmp(const point& pivot) :pivot(pivot) {}
  bool operator()(const point& a, const point& b){
    if (turn(pivot, a, b) == COLLINEAR)
      return dist2(pivot, a) < dist2(pivot, b); // which one closer
    int d1x = a.x - pivot.x, d1y = a.y - pivot.y;
    int d2x = b.x - pivot.x, d2y = b.y - pivot.y;
    return (atan2((double)d1y, (double)d1x)
	    - atan2((double)d2y, (double)d2x)) < 0;
  }
};

//Counterclockwise starting from bottom-most point
void orderByAngle(polygon& p){
  // first, find p0 = point with lowest Y and if tie: rightmost X
  int lowestIndex = 0;
  int n = p.size();

  for (int i = 1; i < n; i++) {
    if (p[i].y < p[lowestIndex].y ||
	(p[i].y == p[lowestIndex].y && p[i].x > p[lowestIndex].x))
      { lowestIndex = i; }
  }

  swap(p[0],p[lowestIndex]);

  // second, sort points by angle w.r.t. lowest
  LeftTurnCmp angle_cmp(p[0]);
  sort(++p.begin(), p.end(), angle_cmp);
}

/* The following code assumes you have ordered the points! */

double perimeter(polygon& p) {
  double result = 0.0;

  //double x1, y1, x2, y2, dx, dy;
  for (int i = 0; i < p.size(); i++) {
    result += distance(p[i],p[(i+1)%p.size()]);
  }

  return result;
}

double determinant(polygon& p) {
  double result = 0;

  double x1, y1, x2, y2;
  for (int i = 0; i < p.size(); i++) {
    x1 = p[i].x; x2 = p[(i + 1) % p.size()].x;
    y1 = p[i].y; y2 = p[(i + 1) % p.size()].y;
    result += (x1 * y2 - x2 * y1);
  }

  return result;
}

// area is half of the determinant and the result may be a non-integer
double area(polygon& p)
{ return fabs(determinant(p)) / 2.0; }

//n must be >= 3 for this method to work
void getConvexHull(polygon& p, polygon& convexHull){
  int n = p.size();

  stack<point> S;
  point prev, now;
  S.push(p[n - 1]); // put two starting vertices into stack S
  S.push(p[0]);

  int i = 1; // and start checking the rest

  while (i < n) {
    // get the 2nd item from top of S
    now = S.top();
    S.pop();
    prev = S.top();
    S.push(now); 

    if (turn(prev, now, p[i]) == LEFT) {
      S.push(p[i]); // accept
      i++;
    }

    // otherwise pop this point until we have a left turn
    else { S.pop(); }
  }

  while (!S.empty()) { // from stack back to vector
    convexHull.push_back(S.top());
    S.pop();
  }

  convexHull.pop_back(); // the last one is a duplicate of first one
}

int main(){
  polygon p = {{0,0}, {5,0}, {0,5}, {2,7}, {5,5} };
  orderByAngle(p);
  cout << perimeter(p) << ' ' << area(p) << endl;
}
