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
  animation->Draw(renderer, camera, Anchor(pos));
}
