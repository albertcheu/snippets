//Adapted from sites.google.com/site/stevenhalim/home/material
//or cpbook.net
#include <cmath>
#include <vector>
#include <stack>
#include <algorithm>
#include <iostream>
#include <cfloat>

#include <cstdio>
#include <cstdlib>
#include <ctime>

#define COLLINEAR 0
#define LEFT 1
#define RIGHT -1
#define polygon vector<point>

using namespace std;
const double EPS = 0.000000001;
const double PI = 3.141592654;

bool equal(double a, double b) { return fabs(a-b) < EPS; }

struct point;
struct line;
struct vec;
struct ray;

// use this whenever possible
//struct point { int x, y
struct point {
  double x,y;
  point();
  point(double _x, double _y);
  point(pair<double,double> p);
  bool operator== (const point& o) const;
  bool operator< (const point& other) const;
  vec operator- (const point& other);
  point operator+ (const vec& v);
  friend ostream& operator<<(ostream& os, const point& pt);
};
bool sortPtsY(point a, point b){ 
  if (fabs(a.y - b.y) > EPS) return a.y < b.y;
  return a.x < b.x; 
}

struct line {
  double a, b, c;
  line (const point& p1, const point& p2);
  bool equivalent(const line& other) const;
  bool operator== (const line& other) const;
  //are they parallel
  bool parallel(const line& other) const;
  bool operator|| (const line& other) const;
  bool has(const point& p) const;
};

//struct vec { int dx, dy;
struct vec {
  double dx, dy, mag;
  vec(double x, double y);
  vec(const point& from, const point& to); // convert 2 points to vector
  vec(pair<double,double> p);

  vec normalize();

  //give the vector pointing to the left
  vec perpendicular();

  //vector addition
  vec operator+ (const vec& other);

  //scale
  vec operator* (double s);

  //dot product
  double operator* (const vec& other);
};

struct ray : public line {
  point origin, dest;
  ray (const point& p1, const point& p2);
};

point::point() :x(0), y(0) {}
point::point(double _x, double _y)
  :x(_x), y(_y)
{}
point::point(pair<double,double> p)
  :x(p.first), y(p.second)
{}
bool point::operator== (const point& o) const
{ return equal(x,o.x) && equal(y,o.y); }
bool point::operator< (const point& other) const{
  if (fabs(x - other.x) > EPS) return x < other.x;
  return y < other.y;
}
vec point::operator- (const point& other){ return vec(other,*this); }
point point::operator+ (const vec& v){ return point(x+v.dx,y+v.dy); }
double distance(const point& p1, const point& p2) {
  return hypot(p1.x - p2.x, p1.y - p2.y);
}
ostream& operator<<(ostream& os, const point& pt){
  os << '(' << pt.x << ',' << pt.y << ')';
  return os;
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

bool line::has(const point& p) const { return equal(a*p.x + b*p.y + c, 0); }

ray::ray(const point& p1, const point& p2)
  :line(p1,p2), origin(p1), dest(p2)
{}

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
vec vec::perpendicular(){
  return vec(-dy,dx);
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

//for convenience, we use a ray to represent line segment
bool rayIntersect(const ray& r, const ray& seg, point& p) {
  if (intersect(r,seg,p)){
    point o = r.origin;
    point d = r.dest;

    point a = seg.origin;
    point b = seg.dest;

    //if we follow the ray's line and turn at the "dest" point to a
    int oda = turn(o,d,a);

    //if we follow the ray's line and turn at the "dest" point to b
    int odb = turn(o,d,b);

    //weird cases: pass thru an endpoint
    if (p == a) { return odb == turn(o,a,b); }
    if (p == b) { return oda == turn(o,b,a); }
    //ray origin is on the segment's line
    if (p == o) { return oda != odb; }

    //if we follow the ray's line and turn at the intersection point to a
    int opa = turn(o,p,a);

    //if we cut through segment ab,
    //turning to a at either d or p should be the same direction
    //and turning to a should be different from turning to b
    return oda == opa && oda != odb;
  }

  return false;
}

//check if the lines are coincident AND the endpoints overlap
bool segmentOverlap(ray& seg1, ray& seg2){
  if (seg1 == seg2) {
    point a = seg1.origin;
    point b = seg1.dest;
    point c = seg2.origin;
    point d = seg2.dest;
    return equal(distance(a,c)+distance(c,b), distance(a,b))
      || equal(distance(c,a)+distance(a,d), distance(c,d));
  }
  return false;
}

/* The following code assumes CLOCKWISE or COUNTERCLOCKWISE ORDER! */

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


double angle(point a, point pivot, point b){
  return (atan2(a.y-pivot.y, a.x-pivot.x) - atan2(b.y-pivot.y, b.x-pivot.x));
}

//ray-casting
bool inPolygon(polygon& p, point& pt){
  //stupid case
  if (pt == p[0]) { return false; }

  //pick arbitrary vertex u
  point u = p[0];
  point v = p[0];
  double minAngle = DBL_MAX;

  //find the vertex v s.t. (angle u-pt-v) is minimal
  //this ensures there are no vertices in between
  for(int i = 1; i < p.size(); i++){
    //stupid case again
    if (p[i] == pt) { return false; }
    double theta = abs(angle(u,pt,p[i]));
    if (theta < minAngle) {
      minAngle = theta;
      v = p[i];
    }
  }

  point alt((u.x + v.x)/2, (u.y + v.y)/2);
  ray r(pt,alt);

  int edgesHit = 0;

  for(int i = 0; i < p.size(); i++){
    point contact;
    point thisPoint = p[i];
    point nextPoint = p[(i+1)%p.size()];

    if (rayIntersect(r, thisPoint, nextPoint, contact)) {
      edgesHit++;
    }
  }

  return (edgesHit % 2);
}


//important angle-sorting functor for convex hull
class AngleCmp{
private:
  point pivot;
public:
  AngleCmp(const point& pivot) :pivot(pivot) {}
  bool operator()(const point& a, const point& b){
    
    if (turn(pivot, a, b) == COLLINEAR) {
      return dist2(pivot, a) < dist2(pivot, b); // which one closer
    }

    return angle(a,pivot,b) < 0;
  }
};

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
  AngleCmp ac(p[0]);
  sort(++p.begin(), p.end(), ac);
}

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

void closestDist(vector<point> pts_x, int left, int right,
		   vector<point> pts_y,
		 point& a, point& b, double& minD){
  if (left-right == 1) { return; }
  if (left-right == 2) {
    double d = distance(pts_x[left],pts_x[left+1]);
    if (d < minD) {
      a = pts_x[left];
      b = pts_x[left+1];
      minD = d;
    }
    return;
  }
  
  //double ans = DBL_MAX;
  
  //recurse
  int mid = (left+right)/2;
  int mid_x = pts_x[mid].x;
  closestDist(pts_x,left,mid,pts_y,a,b,minD);
  closestDist(pts_x,mid,right,pts_y,a,b,minD);
  
  //make strip
  vector<point> strip;
  for(auto pt: pts_y){
    if (pt.x > mid_x-minD && pt.x < mid_x+minD) { strip.push_back(pt); }
  }

  //check strip
  for(int i = 0; i < strip.size(); i++){

    //we'll only check from left side to right side
    if (strip[i].x >= mid_x) { continue; }

    for(int j = 0; j < 4; j++){
      //don't go overboard
      if (i+j >= strip.size()) { break; }

      double d = distance(strip[i],strip[j]);
      if (d < minD) {
	minD = d;
	a = strip[i];
	b = strip[j];
      }
    }
  }

  return;
}

double closestDist(vector<point> pts, point& a, point& b){
  sort(pts.begin(), pts.end());

  vector<point> pts_y;
  copy(pts.begin(), pts.end(), pts_y.begin());
  sort(pts_y.begin(), pts_y.end(), sortPtsY);

  double minD = DBL_MAX;
  closestDist(pts,0,pts.size(),pts_y, a,b, minD);
  return minD;
}

int main(){
  polygon p = {{0,0}, {5,0}, {0,5}, {2,7}, {5,5} };
  orderByAngle(p);
  cout << perimeter(p) << ' ' << area(p) << endl;
}
