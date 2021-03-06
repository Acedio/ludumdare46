#ifndef HERO_H
#define HERO_H

#include <SDL.h>

#include "animation.h"
#include "buttons.h"
#include "camera.h"
#include "drawable.h"
#include "event.h"
#include "geometry.h"
#include "tilemap.h"

enum class Direction {
  LEFT,
  RIGHT,
  UP,
  DOWN,
};

class Hero {
 public:
  std::vector<Event> Update(double t, ButtonState buttons,
                            const TileMap& tilemap);
  Drawable GetDrawable() const;

  Rect BoundingBox() const {
    return bounding_box;
  }
  Vec GetTarget() const;
  const Animation& CurrentSprite() const;

  Hero(SDL_Renderer* renderer, Vec pos)
      : bounding_box{pos.x, pos.y, 0.8, 0.9},
        vel{0, 0},
        up(Animation::LoadFromCSV(renderer, "asset_dir/ghosty_up.anim")),
        down(Animation::LoadFromCSV(renderer, "asset_dir/ghosty_down.anim")),
        left(Animation::LoadFromCSV(renderer, "asset_dir/ghosty_left.anim")),
        right(Animation::LoadFromCSV(renderer, "asset_dir/ghosty_right.anim")) {};

 private:
  Rect bounding_box;
  // Tiles/sec
  Vec vel;

  std::unique_ptr<Animation> up;
  std::unique_ptr<Animation> down;
  std::unique_ptr<Animation> left;
  std::unique_ptr<Animation> right;

  Direction dir = Direction::RIGHT;
};

#endif
