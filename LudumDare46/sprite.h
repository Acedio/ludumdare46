#ifndef SPRITE_H
#define SPRITE_H

#include "camera.h"
#include "tilemap.h"

class Sprite {
 public:
  Sprite(const TileSet* tileset, Tile tile) : tileset(tileset), tile(tile) {}
  void Draw(SDL_Renderer* renderer, const Camera& camera, const SDL_Rect& dst) const;
  void DrawAngle(SDL_Renderer* renderer, const Camera& camera, const SDL_Rect& dst,
                 double rads) const;

 private:
  const TileSet* tileset;  // Not owned.
  Tile tile;
  // TODO: Should probably also have a w and h;
};

#endif
