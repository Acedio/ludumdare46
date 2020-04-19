#include "path.h"

#include <SDL.h>

#include "csvread.h"

Path Path::LoadFromCSV(const std::string& filename) {
  std::unique_ptr<CSVRead> reader = CSVRead::OpenCSV(filename);

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
  SDL_assert(seg + 1 < points.size());
  segment = seg;
  segment_t = 0;
  seg_vec = points[seg + 1] - points[seg];
  double l = Length(seg_vec);
  segment_total_t = l / speed;
}

bool Path::Update(double t) {
  segment_t += t;
  while (segment_t > segment_total_t) {
    double remainder = segment_t - segment_total_t;
    if (segment + 2 >= points.size()) {
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
