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
  animation->w = reader->ReadInt();
  animation->h = reader->ReadInt();
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

bool Animation::Update() {
  ++ticks;
  if (ticks >= frames[frame].ticks) {
    if (frame + 1 < frames.size()) {
      ++frame;
      ticks = 0;
    } else if (loop) {
      frame = 0;
    } else {
      return false;
    }
  }
  return true;
}

SDL_Rect Animation::getFrame(int row, int col) const {
  SDL_Rect src;
  src.x = col * w;
  src.y = row * h;
  src.w = w;
  src.h = h;

  return src;
}

void Animation::Draw(SDL_Renderer* renderer, const Camera& camera, const SDL_Rect& dst) const {
  SDL_Rect src = getFrame(frames[frame].row, frames[frame].col);
  SDL_Rect with_src_bounds = camera.Transform(dst);
  with_src_bounds.w = src.w;
  with_src_bounds.h = src.h;
  SDL_RenderCopy(renderer, tex, &src, &with_src_bounds);
}

void Animation::DrawAngle(SDL_Renderer* renderer, const Camera& camera, const SDL_Rect& dst, double rads) const {
  SDL_Rect src = getFrame(frames[frame].row, frames[frame].col);
  double degrees = rads * (-180 / 3.14159);
  SDL_Rect with_src_bounds = camera.Transform(dst);
  with_src_bounds.w = src.w;
  with_src_bounds.h = src.h;
  SDL_RenderCopyEx(renderer, tex, &src, &with_src_bounds, degrees, NULL, SDL_FLIP_NONE);
}

void Animation::SetLoop(bool l) {
  loop = l;
}

