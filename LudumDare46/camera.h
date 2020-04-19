#ifndef CAMERA_H
#define CAMERA_H

#include <SDL.h>

class Camera {
public:
  Camera(const SDL_Rect& camera, const SDL_Rect& bounds) : camera(camera), fx(camera.x), fy(camera.y), bounds(bounds) {}

  void Update(double t);
  SDL_Rect Transform(const SDL_Rect& rect) const;
  void Focus(int x, int y);
private:
  // Current camera position.
  SDL_Rect camera;
  double fx, fy;
  // Camera won't show outside of these bounds.
  SDL_Rect bounds;
  int focus_x = 0, focus_y = 0;
};

#endif // CAMERA_H