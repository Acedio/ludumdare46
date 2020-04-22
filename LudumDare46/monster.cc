#include "monster.h"

#include "csvread.h"

std::unique_ptr<Monster> Monster::LoadFromCSV(
  SDL_Renderer* renderer, const Path& p, const std::string& filename) {
  auto monster = std::make_unique<Monster>();
  monster->path = p;
  std::unique_ptr<CSVRead> reader = CSVRead::OpenCSV(filename);
  monster->left = Animation::LoadFromCSV(renderer, reader->ReadString());
  monster->right = Animation::LoadFromCSV(renderer, reader->ReadString());
  monster->up = Animation::LoadFromCSV(renderer, reader->ReadString());
  monster->down = Animation::LoadFromCSV(renderer, reader->ReadString());
  return std::move(monster);
}

void Monster::Update(double t) {
  left->Update();
  right->Update();
  up->Update();
  down->Update();
  path.Update(t);
}

Drawable Monster::GetDrawable() const {
  Drawable drawable;
  drawable.animation = &current_animation();
  drawable.anchor = Anchor(path.Location(), XAlignment::CENTER, YAlignment::BOTTOM);
  drawable.y_sort = path.Location().y;
  return drawable;
}

const Animation& Monster::current_animation() const {
  Vec dir = path.Direction();
  if (std::abs(dir.x) > std::abs(dir.y)) {
    if (dir.x > 0) {
      return *right;
    }
    else {
      return *left;
    }
  } else {
    if (dir.y > 0) {
      return *down;
    }
    else {
      return *up;
    }
  }
}
