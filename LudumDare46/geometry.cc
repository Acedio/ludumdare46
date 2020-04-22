#include "geometry.h"

std::ostream& operator<<(std::ostream& o, const Rect& a) {
  return o << "x: " << a.x << " y: " << a.y << " w: " << a.w << " h: " << a.h;
}

double Length(Vec a) {
  return sqrt(a.x * a.x + a.y * a.y);
}

Vec operator-(const Vec& a, const Vec& b) {
  Vec result;
  result.x = a.x - b.x;
  result.y = a.y - b.y;
  return result;
}

Vec operator+(const Vec& a, const Vec& b) {
  Vec result;
  result.x = a.x + b.x;
  result.y = a.y + b.y;
  return result;
}

Vec operator*(double s, const Vec& a) {
  Vec result;
  result.x = a.x * s;
  result.y = a.y * s;
  return result;
}

Vec Rect::PointAt(XAlignment x_align, YAlignment y_align) const {
  Vec p;
  switch (x_align) { 
    case XAlignment::LEFT:
      p.x = x;
      break;
    case XAlignment::CENTER:
      p.x = x + w / 2;
      break;
    case XAlignment::RIGHT:
      p.x = x + w;
      break;
  }
  switch (y_align) { 
    case YAlignment::TOP:
      p.y = y;
      break;
    case YAlignment::CENTER:
      p.y = y + h / 2;
      break;
    case YAlignment::BOTTOM:
      p.y = y + h;
      break;
  }
  return p;
}

bool Intersects(double a1, double a2, double b1, double b2) {
  // NOTE: Collisions are open on the top end, closed on the bottom. [0, 1) does
  // not collide with [1, 2). This way unit squares can be stacked without
  // colliding.
  // To collide, one of the following must be true:
  return
      //  - (B1 (A1 B2) A2) : A1 is within B.
      (a1 >= b1 && a1 < b2) ||
      //  - (A1 (B1 A2) B2) : A2 is within B.
      (a2 >= b1 && a2 < b2) ||
      //  - (A1 (B1 B2) A2) : Both are outside B.
      (a1 < b1 && a2 >= b2);
}

bool Intersects(Rect a, Rect b) {
  bool x = Intersects(a.x, a.x + a.w, b.x, b.x + b.w);
  bool y = Intersects(a.y, a.y + a.h, b.y, b.y + b.h);
  return x && y;
}
