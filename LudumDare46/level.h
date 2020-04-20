#ifndef LEVEL_H
#define LEVEL_H

#include <string>
#include <optional>

#include "geometry.h"

struct Level {
  Vec start;
  // Paths, monster waves, etc.
};

std::optional<Level> LoadLevelFromCSV(const std::string& filename);

#endif // LEVEL_H