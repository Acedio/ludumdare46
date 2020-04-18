#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <iostream>

struct Rect {
  double x;
  double y;
  double w;
  double h;
};

std::ostream& operator<<(std::ostream& o, const Rect& a);

struct Vec {
  double x;
  double y;
};

bool Intersects(Rect a, Rect b);
// Two segments on a line.
bool Intersects(int a1, int a2, int b1, int b2);

#endif
