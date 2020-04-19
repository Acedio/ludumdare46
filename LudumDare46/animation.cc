#include "animation.h"

#include <SDL_image.h>

#include "csvread.h"

std::unique_ptr<Animation> Animation::LoadFromCSV(SDL_Renderer* renderer, const std::string& filename) {
  std::unique_ptr<CSVRead> reader = CSVRead::OpenCSV(filename);
  SDL_assert(reader != nullptr);
  std::unique_ptr<Animation> animation(new Animation);
  SDL_assert(animation != nullptr);
  std::string spritefile = reader->ReadString();
  animation->tex = IMG_LoadTexture(renderer, spritefile.c_str());
  animation->width = reader->ReadInt();
  animation->height = reader->ReadInt();
  int frames = reader->ReadInt();

  for (int i = 0; i < frames; ++i) {
    Animation::Frame frame;
    frame.ticks = reader->ReadInt();
    frame.row = reader->ReadInt();
    frame.col = reader->ReadInt();
    animation->frames.push_back(frame);
  }
  return animation;
}

void Animation::Update() {
  ++ticks;
  if (ticks >= frames[frame].ticks) {
    ticks = 0;
    ++frame;
    if (frame >= frames.size()) {
      frame = 0;
    }
  }
}

SDL_Rect Animation::getFrame(int row, int col) const {
  SDL_Rect src;
  src.x = col * width;
  src.y = row * height;
  src.w = width;
  src.h = height;

  return src;
}

void Animation::Draw(SDL_Renderer* renderer, const SDL_Rect& dst) const {
  SDL_Rect src = getFrame(frames[frame].row, frames[frame].col);
  SDL_RenderCopy(renderer, tex, &src, &dst);
}

void Animation::DrawAngle(SDL_Renderer* renderer, const SDL_Rect& dst, double rads) const {
  SDL_Rect src = getFrame(frames[frame].row, frames[frame].col);
  double degrees = rads * (-180 / 3.14159);
  SDL_RenderCopyEx(renderer, tex, &src, &dst, degrees, NULL, SDL_FLIP_NONE);
}
