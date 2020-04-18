#ifndef BUTTONS_H
#define BUTTONS_H

struct ButtonState {
  bool left;
  bool right;
  bool jump;
  bool grab;
  bool restart;
  bool exit;
};

ButtonState GetButtonState();

#endif
