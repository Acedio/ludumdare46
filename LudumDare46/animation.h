#ifndef ANIMATION_H
#define ANIMATION_H

#include <SDL.h>

#include <string>
#include <memory>
#include <vector>

class Animation {
public:
  static std::unique_ptr<Animation> LoadFromCSV(SDL_Renderer* renderer, const std::string& filename);

  void Update();

  void Draw(SDL_Renderer* renderer, const SDL_Rect& dst) const;
  void DrawAngle(SDL_Renderer* renderer, const SDL_Rect& dst,
                 double rads) const;
private:
  Animation() {}

  SDL_Rect getFrame(int row, int col) const;

  struct Frame {
    int ticks;
    int col;
    int row;
  };

  SDL_Texture *tex;

  std::vector<Frame> frames;
  int frame = 0;
  int ticks = 0;

  int width;
  int height;
};

#endif  // ANIMATION_H