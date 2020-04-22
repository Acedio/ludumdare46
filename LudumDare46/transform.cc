#include "transform.h"

IVec ToPixelSpace(const Vec& pos) {
  return IVec{int(pos.x * kTileWidth), int(pos.y * kTileHeight)};
}

SDL_Rect ToPixelSpace(const Rect& rect) {
  SDL_Rect sr;
  sr.x = rect.x * kTileWidth;
  sr.y = rect.y * kTileHeight;
  sr.w = kTileWidth;
  sr.h = kTileHeight;
  return sr;
}

Vec ToGameSpace(const IVec& pos) {
  return Vec{(double)pos.x / kTileWidth, (double)pos.y / kTileHeight};
}

SDL_Rect Anchor::PixelSpace(IVec pixel_dimensions) const {
  IVec pixel_pos = ToPixelSpace(game_pos);
  SDL_Rect pixel_space;
  pixel_space.w = pixel_dimensions.x;
  pixel_space.h = pixel_dimensions.y;
  switch (x_align) { 
    case XAlignment::LEFT:
      pixel_space.x = pixel_pos.x;
      break;
    case XAlignment::CENTER:
      pixel_space.x = pixel_pos.x - pixel_space.w / 2;
      break;
    case XAlignment::RIGHT:
      pixel_space.x = pixel_pos.x - pixel_space.w;
      break;
  }
  switch (y_align) { 
    case YAlignment::TOP:
      pixel_space.y = pixel_pos.y;
      break;
    case YAlignment::CENTER:
      pixel_space.y = pixel_pos.y - pixel_space.h / 2;
      break;
    case YAlignment::BOTTOM:
      pixel_space.y = pixel_pos.y - pixel_space.h;
      break;
  }
  return pixel_space;
}
