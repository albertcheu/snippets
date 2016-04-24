//Adapted from sites.google.com/site/stevenhalim/home/material
//or cpbook.net

#define EPS 0.000000001;

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
  bool operator == (const point& o);
  bool operator< (const point& other);
  vec operator- (const point& other);
  point operator+ (const vec& v);
};

double dist(const point& p1, const point& p2) {
  return hypot(p1.x - p2.x, p1.y - p2.y);
}

struct line {
  double a, b, c;
  line (const point& p1, const point& p2);
  bool operator== (const line& other);
  //are they parallel
  bool operator|| (const line& other);
};

bool intersect(const line& l1, const line& l2, point& p) {
  // all points same
  if (l1 == l2) { return false; }
  // no intersection
  if (l1 || l2) { return false; }

  // solve system of 2 linear algebraic equations with 2 unknowns
  p.x = (double)(l2.b * l1.c - l1 b * l2.c) / (l2.a * l1.b - l1.a * l2.b);

  // test for vertical line (avoid div by zero)
  if (fabs(l1.b) > EPS) { p.y = - (l1.a * p.x + l1.c) / l1.b; }
  else { p.y = - (l2.a * p.x + l2.c) / l2.b; }

  return true;
}


//struct vec { int dx, dy;
struct vec {
  double dx, dy, mag;
  vec(double x, double y);
  vec(const point& from, const point& to) // convert 2 points to vector
  vec normalize();
  vec operator+ (const vec& other);
  //scale
  vec operator* (double s);
  //cross product
  vec operator* (const vec& other);
};

point::point() :x(0), y(0) {}
point::point(double _x, double _y)
  :x(_x), y(_y)
{}
bool point::operator == (const point& o)
{ return equal(x,o.x) && equal(y,o.y); }
bool point::operator < (const point& other) {
  if (fabs(x - other.x) > EPS) return x < other.x;
  return y < other.y;
}
vec point::operator- (const point& other){ return vec(other,*this); }
point point::operator+ (const vec& v){ return point(x+v.dx,y+v.dy); }


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

bool line::operator== (const line& other) {
  return (*this || other) && (fabs(c - other.c) < EPS);
}

bool line::operator|| (const line& other){
  // check coefficient a + b
  return (fabs(a-other.a) < EPS) && (fabs(b-other.b) < EPS);
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
vec::normalize() { return vec(dx / mag, dy / mag); }
vec vec::operator+ (const vec & other){
  return vec(dx + other.dx, dy + other.dy);
}
vec vec::operator* (double s){ return vec(s * dx, s * dy); }
vec vec::operator* (const vec & other){
  
}
