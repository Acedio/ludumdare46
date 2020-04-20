#ifndef LEVEL_H
#define LEVEL_H

#include <SDL.h>
#include <string>
#include <optional>
#include <vector>

#include "geometry.h"
#include "path.h"
#include "monster.h"

struct Level {
  Vec start;
  std::vector<Path> paths;
  std::vector<Monster> monsters;
};

std::optional<Level> LoadLevelFromCSV(SDL_Renderer* renderer, const std::string& filename);

#endif // LEVEL_H