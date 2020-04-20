#include "path.h"

#include <SDL.h>

#include "csvread.h"

Path Path::LoadFromCSV(CSVRead* reader) {
  Path path;
  int segments = reader->ReadInt();
  for (int i = 0; i < segments; ++i) {
    Vec p;
    p.x = reader->ReadInt();
    p.y = reader->ReadInt();
    path.points.push_back(p);
  }

  path.Reset();
  return path;
}

void Path::Reset() {
  LoadSegment(0);
}

void Path::LoadSegment(int seg) {
  if (seg + 1 < points.size()) {
    segment = seg;
    segment_t = 0;
    seg_vec = points[seg + 1] - points[seg];
    double l = Length(seg_vec);
    segment_total_t = l / speed;
  } else {
    // Pause at the end of the last segment.
    segment = points.size() - 1;
    seg_vec = { 0, 0 };
  }
}

bool Path::Update(double t) {
  segment_t += t;
  while (segment_t > segment_total_t) {
    double remainder = segment_t - segment_total_t;
    if (segment + 1 >= points.size()) {
      return true;
    }
    LoadSegment(segment + 1);
    segment_t = remainder;
  }
  return false;
}

Vec Path::Location() const {
  return points[segment] + (segment_t / segment_total_t) * seg_vec;
}
