#ifndef PATH_H
#define PATH_H

#include <string>
#include <vector>

#include "geometry.h"
#include "csvread.h"

class Path {
public:
  static Path LoadFromCSV(CSVRead* reader);

  void Reset();
  // Returns true if the end has been reached.
  bool Update(double t);

  // TODO: Probably don't want to use this directly since it will look robotic.
  // Add some sort of modulation. Or have monsters loosely follow this as a carrot.
  Vec Location() const;

private:
  void LoadSegment(int seg);

  std::vector<Vec> points;

  int segment = 0;
  Vec seg_vec;
  double segment_t = 0;
  double segment_total_t = 0;
  double speed = 16;
};

#endif // PATH_H