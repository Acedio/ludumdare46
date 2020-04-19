#include "camera.h"

void Camera::Update(double t) {
  double target_x = focus_x - camera.w / 2;
  double target_y = focus_y - camera.h / 2;
  fx += t * (target_x - fx);
  fy += t * (target_y - fy);

  camera.x = fx;
  camera.y = fy;

  if (camera.x < bounds.x) {
    camera.x = bounds.x;
    fx = camera.x;
  } else if (camera.x + camera.w > bounds.x + bounds.w) {
    camera.x = bounds.x + bounds.w - camera.w;
    fx = camera.x;
  }
  if (camera.y < bounds.y) {
    camera.y = bounds.y;
    fy = camera.y;
  } else if (camera.y + camera.h > bounds.y + bounds.h) {
    camera.y = bounds.y + bounds.h - camera.h;
    fy = camera.y;
  }
}

SDL_Rect Camera::Transform(const SDL_Rect& rect) const {
  SDL_Rect transformed = rect;
  transformed.x -= camera.x;
  transformed.y -= camera.y;
  return transformed;
}

void Camera::Focus(int x, int y) {
  focus_x = x;
  focus_y = y;
}
