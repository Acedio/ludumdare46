#include "geometry.h"

std::ostream& operator<<(std::ostream& o, const Rect& a) {
  return o << "x: " << a.x << " y: " << a.y << " w: " << a.w << " h: " << a.h;
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
