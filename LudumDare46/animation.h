#ifndef ANIMATION_H
#define ANIMATION_H

#include <SDL.h>

#include <string>
#include <memory>
#include <vector>

#include "camera.h"

class Animation {
public:
  static std::unique_ptr<Animation> LoadFromCSV(SDL_Renderer* renderer, const std::string& filename);

  // Returns false if the animation has finished.
  bool Update();

  void Draw(SDL_Renderer* renderer, const Camera& camera, const SDL_Rect& dst) const;
  void DrawAngle(SDL_Renderer* renderer, const Camera& camera, const SDL_Rect& dst,
                 double rads) const;

  void SetLoop(bool l);

  int width() const { return w; }
  int height() const { return h; }
private:
  Animation() {}

  SDL_Rect getFrame(int row, int col) const;

  struct Frame {
    int ticks;
    int col;
    int row;
  };

  SDL_Texture *tex = nullptr;

  std::vector<Frame> frames;
  int frame = 0;
  int ticks = 0;
  bool loop = true;

  int w = 0;
  int h = 0;
};

#endif  // ANIMATION_H