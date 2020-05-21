#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include <memory>
#include <SDL.h>
#include <SDL_mixer.h>

#include "buttons.h"
#include "game.h"

// TODO: Move these into a game state that can be handled by Emscripten.
SDL_Window* global_window;
SDL_Renderer* global_renderer;
Uint32 next_frame;
std::unique_ptr<Game> game;

const double kTimestep = 1.0 / 60.0;

void gameLoop() {
  // Update game logic.
  while (next_frame < SDL_GetTicks()) {
    ButtonState buttons = GetButtonState();
    if (!game->Update(global_renderer, buttons, kTimestep)) {
      return;
    }
    next_frame += 1000 * kTimestep;
  }

  // Draw.
  game->Draw(global_renderer);
  SDL_RenderPresent(global_renderer);
}

int main(int argc, char** argv) {
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
    return -1;
  }

  // TODO: Set up a context to remove the globals.
  global_window = SDL_CreateWindow("Ludum Dare 46", 0, 0, 640, 480, 0);
  if (!global_window) {
    return -1;
  }
  global_renderer = SDL_CreateRenderer(global_window, -1, 0);
  if (!global_renderer) {
    return -1;
  }
  SDL_RenderSetScale(global_renderer, 2, 2);
  SDL_RenderSetIntegerScale(global_renderer, SDL_TRUE);

  game = Game::Load(global_renderer);
  SDL_assert(game != nullptr);

  next_frame = SDL_GetTicks();

#ifdef __EMSCRIPTEN__
  emscripten_set_main_loop(gameLoop, -1, true);
#else
  while (true) {
    gameLoop();
  }
#endif

  SDL_DestroyRenderer(global_renderer);
  SDL_DestroyWindow(global_window);
  SDL_Quit();

  return 0;
}
