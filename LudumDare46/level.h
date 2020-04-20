#ifndef LEVEL_H
#define LEVEL_H

#include <SDL.h>
#include <string>
#include <memory>
#include <vector>

#include "geometry.h"
#include "path.h"
#include "monster.h"

struct Level {
  Vec start;
  std::vector<std::unique_ptr<Monster>> monsters;
};

std::unique_ptr<Level> LoadLevelFromCSV(SDL_Renderer* renderer, const std::string& filename);

#endif // LEVEL_H