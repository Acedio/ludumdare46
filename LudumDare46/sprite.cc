#include "sprite.h"

void Sprite::Draw(SDL_Renderer* renderer, const Camera& camera, const SDL_Rect& dst) const {
  SDL_assert(tileset);
  tileset->DrawTile(renderer, camera, tile, dst);
}

void Sprite::DrawAngle(SDL_Renderer* renderer, const Camera& camera, const SDL_Rect& dst,
                       double rads) const {
  SDL_assert(tileset);
  tileset->DrawTileAngle(renderer, camera, tile, dst, rads);
}
