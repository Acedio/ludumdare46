#ifndef MONSTER_H
#define MONSTER_H

#include <memory>
#include <SDL.h>

#include "animation.h"
#include "path.h"

class Monster {
public:
  Monster(const Path& path, std::unique_ptr<Animation> animation) : path(path), animation(std::move(animation)) {}
  void Update(double t);
  void Draw(SDL_Renderer* renderer, const Camera& camera) const;

private:
  Path path;
  std::unique_ptr<Animation> animation;
};

#endif // MONSTER_H