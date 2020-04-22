#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <cmath>
#include <iostream>

struct Vec {
  double x;
  double y;
};

double Length(Vec a);
Vec operator-(const Vec& a, const Vec& b);
Vec operator+(const Vec& a, const Vec& b);
Vec operator*(double s, const Vec& a);

struct IVec {
  int x;
  int y;
};

enum class XAlignment {
  LEFT,
  CENTER,
  RIGHT,
};

enum class YAlignment {
  TOP,
  CENTER,
  BOTTOM,
};

struct Rect {
  double x;
  double y;
  double w;
  double h;

  Vec TopLeft() const { return Vec{x, y}; }
  Vec PointAt(XAlignment x_align, YAlignment y_align) const ;
  Vec Dimensions() const { return Vec{w, h}; }
};

std::ostream& operator<<(std::ostream& o, const Rect& a);

bool Intersects(Rect a, Rect b);
// Two segments on a line.
bool Intersects(int a1, int a2, int b1, int b2);

#endif
