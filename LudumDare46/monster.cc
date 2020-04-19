#include "monster.h"

void Monster::Update(double t) {
  animation->Update();
  path.Update(t);
}
void Monster::Draw(SDL_Renderer* renderer, const Camera& camera) const {
  SDL_Rect dst;
  dst.w = animation->width();
  dst.h = animation->height();
  // Center it on the path location.
  Vec loc = path.Location();
  dst.x = loc.x - dst.w/2;
  dst.y = loc.y - dst.h/2;
  animation->Draw(renderer, camera, dst);
}
