#ifndef DRAWABLE_H
#define DRAWABLE_H

#include "animation.h"
#include "transform.h"

struct Drawable {
  void Draw(SDL_Renderer* renderer, const Camera& camera) const;

  const Animation* animation;
  Anchor anchor;
  double y_sort;
};

struct DrawableOrder {
  bool operator()(const Drawable& a, const Drawable& b) {
    return a.y_sort < b.y_sort;
  }
};

#endif // DRAWABLE_H