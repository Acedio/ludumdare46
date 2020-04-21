#include "tower.h"

#include "tilemap.h"

void Tower::Update() {
  if (creating && !create->Update()) {
    creating = false;
  } else {
    idle->Update();
  }
}

void Tower::Draw(SDL_Renderer* renderer, const Camera& camera) const {
  const Animation* animation;
  if (creating) {
    animation = create.get();
  } else {
    animation = idle.get();
  }
  Vec pixel_space = ToPixelSpace(pos);
  SDL_Rect dst;
  dst.w = animation->width();
  dst.h = animation->height();
  // Center it on the location.
  dst.x = pixel_space.x - dst.w/2;
  dst.y = pixel_space.y - dst.h/2;
  animation->Draw(renderer, camera, dst);
}
