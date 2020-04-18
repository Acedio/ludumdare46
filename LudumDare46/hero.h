#ifndef HERO_H
#define HERO_H

#include <SDL.h>

#include "box.h"
#include "buttons.h"
#include "event.h"
#include "geometry.h"
#include "sprite.h"
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
  void Draw(SDL_Renderer* renderer) const;

  Rect BoundingBox() const {
    return bounding_box;
  }
  const Sprite& CurrentSprite() const;

  Hero(const TileSet* tileset, Vec pos)
      : bounding_box{pos.x, pos.y, 0.8, 0.9},
        vel{0, 0},
        left(tileset, 4),
        right(tileset, 5) {};

 private:
  void UpdateGrab(ButtonState buttons, const TileMap& tilemap,
                  BoxManager* boxes);
  Vec HeldUpperLeft() const;

  Rect bounding_box;
  // Tiles/sec
  Vec vel;

  Sprite left;
  Sprite right;

  bool facing_right = true;
  JumpState jump_state = JumpState::RECOVERING;

  GrabState grab_state = GrabState::RECOVERING;
  std::optional<GrabbedBox> holding;
};

#endif
