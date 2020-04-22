#include "path.h"

#include <SDL.h>

#include "csvread.h"
#include "transform.h"

Path Path::LoadFromCSV(CSVRead* reader) {
  Path path;
  int segments = reader->ReadInt();
  for (int i = 0; i < segments; ++i) {
    // The points are in pixel space, store them in game space.
    IVec pixel_space;
    pixel_space.x = reader->ReadInt();
    pixel_space.y = reader->ReadInt();
    path.points.push_back(ToGameSpace(pixel_space));
  }

  path.Reset();
  return path;
}

void Path::Reset() {
  LoadSegment(0);
}

void Path::LoadSegment(int seg) {
  segment = seg;
  if (!Done()) {
    segment_t = 0;
    seg_vec = points[seg + 1] - points[seg];
    double l = Length(seg_vec);
    segment_total_t = l / speed;
  }
}

bool Path::Done() const {
  return segment + 1 >= points.size();
}

bool Path::Update(double t) {
  if (Done()) {
    return true;
  }
  segment_t += t;
  while (segment_t > segment_total_t) {
    double remainder = segment_t - segment_total_t;
    LoadSegment(segment + 1);
    segment_t = remainder;
  }
  return Done();
}

Vec Path::Location() const {
  if (Done()) {
    return points[points.size() - 1];
  } else {
    return points[segment] + (segment_t / segment_total_t) * seg_vec;
  }
}

Vec Path::Direction() const {
  if (Done()) {
    return { 0,1 };
  } else {
    return (1 / Length(seg_vec)) * seg_vec;
  }
}
