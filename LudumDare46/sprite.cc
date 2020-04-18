#include "sprite.h"

void Sprite::Draw(SDL_Renderer* renderer, const SDL_Rect& dst) const {
  SDL_assert(tileset);
  tileset->DrawTile(renderer, tile, dst);
}

void Sprite::DrawAngle(SDL_Renderer* renderer, const SDL_Rect& dst,
                       double rads) const {
  SDL_assert(tileset);
  tileset->DrawTileAngle(renderer, tile, dst, rads);
}
