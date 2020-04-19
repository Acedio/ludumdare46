#include "box.h"

#include <cmath>
#include <iostream>
#include <algorithm>

const double kGravityAcc = 5.0;
const double kSize = 1;
const double kBuffer = 0.001;

void BoxManager::Add(Vec pos) {
  int col = pos.x + 0.5;
  SDL_assert(col >= 0 && col < columns.size());
  columns[col].push_back({floor(pos.y + 0.5)});
  std::sort(columns[col].begin(), columns[col].end(),
            [](const Box& a, const Box& b) { return a.y > b.y; });
}

bool BoxManager::TryAdd(const TileMap& tilemap, Vec upper_left,
                        GrabbedBox box) {
  Rect pos;
  pos.x = floor(upper_left.x + 0.5);
  pos.y = upper_left.y;
  pos.w = 1;
  pos.h = 1;

  auto maybe_correction = YCollide(pos, 0);
  if (maybe_correction) {
    return false;
  }
  // Make sure it's not colliding with the tilemap.
  if (tilemap.CollidingWith(pos) != std::set<TileType>{TileType::NONE}) {
    return false;
  }
  Add(upper_left);
  return true;
}

Rect ToBoundingBox(int col, const Box& box) {
  return Rect{float(col), box.y, kSize, kSize};
}

void BoxManager::Update(double t, const TileMap& tilemap) {
  for (int col = 0; col < columns.size(); ++col) {
    std::vector<Box>& column = columns[col];
    // Go from bottom to top.
    std::sort(column.begin(), column.end(),
              [](const Box& a, const Box& b) { return a.y > b.y; });
    for (int i = 0; i < column.size(); ++i) {
      Box& box = column[i];
      if (!box.stopped) {
        box.y_vel += t*kGravityAcc;
        // Boxes should only fall.
        SDL_assert(box.y_vel >= 0);
        double dy = t*box.y_vel;
        if (i > 0 && box.y + kSize >= column[i - 1].y) {
          // Collided with below box.
          if (column[i - 1].stopped) {
            column[i - 1].stacked_on = true;
          }
          box.stopped = true;
          box.y = column[i-1].y - kSize;
          box.y_vel = 0;
        } else if (CollisionInfo info =
                       tilemap.YCollide(ToBoundingBox(col, box), dy);
                   !info.types.empty()) {
          // Collided with tilemap.
          box.stopped = true;
          box.y += dy + info.correction;
          box.y_vel = 0;
        } else {
          box.y += dy;
        }
      }
    }
  }
}

void BoxManager::Draw(SDL_Renderer* renderer, const Camera& camera) const {
  for (int col = 0; col < columns.size(); ++col) {
    for (const Box& box : columns[col]) {
      SDL_Rect dst = ToSDLRect(ToBoundingBox(col, box));
      sprite.Draw(renderer, camera, dst);
    }
  }
}

bool BoxManager::AtPoint(Vec p) const {
  int col = (int)p.x;
  if (col < 0 || col >= columns.size()) {
    return false;
  }
  // Should always be sorted, go from bottom to top.
  for (const Box& box : columns[col]) {
    if (p.y >= box.y + kSize) {
      // We're below the box, and boxes will only get higher.
      return false;
    }
    if (p.y >= box.y) {
      // Between the top and bottom of the box, collision.
      return true;
    }
  }
  return false;
}

std::optional<GrabbedBox> BoxManager::GrabAt(Vec pos) {
  int col = (int)pos.x;
  if (col < 0 || col >= columns.size()) {
    return std::nullopt;
  }
  // Should always be sorted, go from bottom to top.
  for (int i = 0; i < columns[col].size(); ++i) {
    Box& box = columns[col][i];
    if (pos.y >= box.y + kSize) {
      // We're below the box, and boxes will only get higher.
      return std::nullopt;
    }
    // TODO: This seems brittle :P
    if (pos.y >= box.y && box.stopped && !box.stacked_on) {
      // There is indeed a box to grab.
      if (i > 0) {
        // Unstacked_on the below box if needed.
        columns[col][i-1].stacked_on = false;
      }
      GrabbedBox grabbed{box.type, sprite};
      columns[col].erase(columns[col].begin() + i);
      return grabbed;
    }
  }
  return std::nullopt;
}

std::optional<double> BoxManager::XCollide(const Rect& rect, double dx) const {
  double x1 = rect.x + dx;
  double y1 = rect.y;
  double x2 = rect.x + rect.w + dx;
  double y2 = rect.y + rect.h;

  if (dx <= 0) {
    // Velocity is negative, check left side.
    if (AtPoint({x1, y1})) {
      return ceil(x1) - x1;
    } else if (AtPoint({x1, y2 - kBuffer})) {
      return ceil(x1) - x1;
    }
    return std::nullopt;
  } else {
    // Velocity is positive, check right side.
    if (AtPoint({x2 - kBuffer, y1})) {
      return floor(x2) - x2;
    } else if (AtPoint({x2 - kBuffer, y2 - kBuffer})) {
      return floor(x2) - x2;
    }
    return std::nullopt;
  }
}

std::optional<double> BoxManager::YCollide(const Rect& rect, double dy) const {
  double x1 = rect.x;
  double y1 = rect.y + dy;
  double x2 = rect.x + rect.w;
  double y2 = rect.y + rect.h + dy;

  if (dy <= 0) {
    // Velocity is negative, check top side;
    if (AtPoint({x1, y1})) {
      return ceil(y1) - y1;
    } else if (AtPoint({x2 - kBuffer, y1})) {
      return ceil(y1) - y1;
    }
  }
  // Both arms need to execute for TryAdd().
  if (dy >= 0) {
    // Velocity is positive, check bottom side.
    if (AtPoint({x1, y2 - kBuffer})) {
      return floor(y2) - y2;
    } else if (AtPoint({x2 - kBuffer, y2 - kBuffer})) {
      return floor(y2) - y2;
    }
  }
  return std::nullopt;
}
