#include "buttons.h"

#include <SDL.h>

ButtonState GetButtonState() {
  ButtonState buttons;

  SDL_PumpEvents();

  const Uint8* keys = SDL_GetKeyboardState(nullptr);
  buttons.left = keys[SDL_SCANCODE_LEFT];
  buttons.right = keys[SDL_SCANCODE_RIGHT];
  buttons.up = keys[SDL_SCANCODE_UP];
  buttons.down = keys[SDL_SCANCODE_DOWN];
  buttons.jump = keys[SDL_SCANCODE_Z];
  buttons.grab = keys[SDL_SCANCODE_X];
  buttons.restart = keys[SDL_SCANCODE_R];
  buttons.exit = keys[SDL_SCANCODE_ESCAPE];

  return buttons;
}
