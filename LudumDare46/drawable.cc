#include "drawable.h"

void Drawable::Draw(SDL_Renderer* renderer, const Camera& camera) const {
  animation->Draw(renderer, camera, anchor);
}
