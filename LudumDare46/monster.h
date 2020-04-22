#ifndef MONSTER_H
#define MONSTER_H

#include <memory>
#include <SDL.h>

#include "animation.h"
#include "path.h"
#include "drawable.h"

class Monster {
public:
  static std::unique_ptr<Monster> LoadFromCSV(
    SDL_Renderer *renderer, const Path& p, const std::string& filename);
  void Update(double t);
  Drawable GetDrawable() const;

private:
  const Animation& current_animation() const;

  Path path;
  std::unique_ptr<Animation> left, right, up, down;
};

#endif // MONSTER_H