#ifndef TOWER_H
#define TOWER_H

#include "animation.h"
#include "geometry.h"

class Tower {
 public:
  Tower(Vec pos, std::unique_ptr<Animation> c,
        std::unique_ptr<Animation> i)
      : pos(pos), create(std::move(c)), idle(std::move(i)) {
    create->SetLoop(false);
  }

  void Update();
  void Draw(SDL_Renderer* renderer, const Camera& camera) const;

 private:
  bool creating = true;
  Vec pos;
  std::unique_ptr<Animation> create, idle;
};

#endif // TOWER_H