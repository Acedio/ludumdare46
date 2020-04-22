#include "hero.h"

#include <iostream>

const double kVel = 7.0;
const double kGravityAcc = 50.0;

std::vector<Event> Hero::Update(double t, ButtonState buttons,
                                const TileMap& tilemap) {
  std::vector<Event> events;

  if (buttons.up == buttons.down) {
    vel.y = 0;
  }
  else if (buttons.up) {
    dir = Direction::UP;
    vel.y = -kVel;
  }
  else if (buttons.down) {
    dir = Direction::DOWN;
    vel.y = kVel;
  }

  // MOVEMENT
  if (buttons.left == buttons.right) {
    vel.x = 0;
  } else if (buttons.left) {
    dir = Direction::LEFT;
    vel.x = -kVel;
  } else if (buttons.right) {
    dir = Direction::RIGHT;
    vel.x = kVel;
  }

  double dx = t*vel.x;

  if (CollisionInfo info = tilemap.XCollide(bounding_box, dx);
      info.types.empty()) {
    bounding_box.x += dx;
  } else {
    // Tilemap collision.
    bounding_box.x += dx + info.correction;
    vel.x = 0;
  }

  double dy = t*vel.y;

  if (CollisionInfo info = tilemap.YCollide(bounding_box, dy);
      info.types.empty()) {
    bounding_box.y += dy;
  } else {
    // Tilemap collision.
    bounding_box.y += dy + info.correction;
    vel.y = 0;
  }

  up->Update();
  down->Update();
  left->Update();
  right->Update();

  return events;
}

const Animation& Hero::CurrentSprite() const {
  switch (dir) {
  case Direction::UP:
    return *up;
  case Direction::DOWN:
    return *down;
  case Direction::LEFT:
    return *left;
  case Direction::RIGHT:
    return *right;
  }
}

void Hero::Draw(SDL_Renderer* renderer, const Camera& camera) const {
  // The sprite is a unit square that should be bottom aligned and horizontally
  // centered around the bounding_box.
  CurrentSprite().Draw(
      renderer, camera,
      Anchor(bounding_box.PointAt(XAlignment::CENTER, YAlignment::BOTTOM),
             XAlignment::CENTER,
             YAlignment::BOTTOM));
}
