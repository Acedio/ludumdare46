#ifndef HERO_H
#define HERO_H

#include <SDL.h>

#include "box.h"
#include "buttons.h"
#include "camera.h"
#include "event.h"
#include "geometry.h"
#include "animation.h"
#include "tilemap.h"

enum class JumpState {
  RECOVERING,
  CAN_JUMP,
  FALLING,
};

enum class GrabState {
  RECOVERING,
  CAN_GRAB,
  // Grabbed something and haven't released grab yet.
  GRABBING,
  CAN_RELEASE,
};

class Hero {
 public:
  std::vector<Event> Update(double t, ButtonState buttons,
                            const TileMap& tilemap, BoxManager* boxes);
  void Draw(SDL_Renderer* renderer, const Camera& camera) const;

  Rect BoundingBox() const {
    return bounding_box;
  }
  const Animation& CurrentSprite() const;

  Hero(SDL_Renderer* renderer, Vec pos)
      : bounding_box{pos.x, pos.y, 0.8, 0.9},
        vel{0, 0},
        left(Animation::LoadFromCSV(renderer, "asset_dir/ghosty_left.anim")),
        right(Animation::LoadFromCSV(renderer, "asset_dir/ghosty_right.anim")) {};

 private:
  void UpdateGrab(ButtonState buttons, const TileMap& tilemap,
                  BoxManager* boxes);
  Vec HeldUpperLeft() const;

  Rect bounding_box;
  // Tiles/sec
  Vec vel;

  std::unique_ptr<Animation> left;
  std::unique_ptr<Animation> right;

  bool facing_right = true;
  JumpState jump_state = JumpState::RECOVERING;

  GrabState grab_state = GrabState::RECOVERING;
  std::optional<GrabbedBox> holding;
};

#endif
