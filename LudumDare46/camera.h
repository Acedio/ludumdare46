#ifndef CAMERA_H
#define CAMERA_H

#include <SDL.h>

#include "geometry.h"

class Camera {
public:
  Camera(const SDL_Rect& camera, const SDL_Rect& bounds) : camera(camera), fx(camera.x), fy(camera.y), bounds(bounds) {}

  void Update(double t);
  SDL_Rect Transform(const SDL_Rect& rect) const;
  void Focus(IVec pixel_pos);
private:
  // Current camera position.
  SDL_Rect camera;
  double fx, fy;
  // Camera won't show outside of these bounds.
  SDL_Rect bounds;
  IVec focus;
};

#endif // CAMERA_H