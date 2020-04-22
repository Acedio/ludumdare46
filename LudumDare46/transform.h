#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <SDL.h>

#include "geometry.h"

const int kTileWidth = 8;
const int kTileHeight = 8;

SDL_Rect ToPixelSpace(const Rect& rect);
IVec ToPixelSpace(const Vec& pos);

Vec ToGameSpace(const IVec& pos);

// Fixes a pixel-space texture to a game-space point.
class Anchor {
 public:
  Anchor(Vec game_pos = {0, 0}, XAlignment x_align = XAlignment::LEFT,
         YAlignment y_align = YAlignment::TOP)
      : game_pos(game_pos), x_align(x_align), y_align(y_align) {}

  SDL_Rect PixelSpace(IVec pixel_dimensions) const;

 private:
  Vec game_pos;
  XAlignment x_align;
  YAlignment y_align;
};

#endif // TRANSFORM_H