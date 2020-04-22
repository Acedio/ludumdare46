#include "tower.h"

void Tower::Update() {
  if (creating && !create->Update()) {
    creating = false;
  } else {
    idle->Update();
  }
}

Drawable Tower::GetDrawable() const {
  Drawable drawable;
  if (creating) {
    drawable.animation = create.get();
  } else {
    drawable.animation = idle.get();
  }
  drawable.anchor = Anchor(pos, XAlignment::CENTER, YAlignment::CENTER);
  drawable.y_sort = pos.y; // TODO
  return drawable;
}
