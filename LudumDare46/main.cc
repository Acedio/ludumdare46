#include <memory>
#include <SDL.h>
#include <SDL_mixer.h>

#include "buttons.h"
#include "game.h"

SDL_Window* global_window;
SDL_Renderer* global_renderer;

std::unique_ptr<Game> game;

const double kTimestep = 1.0 / 60.0;

bool gameLoop() {

  return true;
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

  game = Game::Load(global_renderer);
  SDL_assert(game != nullptr);

  Uint32 next_frame = SDL_GetTicks();

  while (true) {
    // Update game logic.
    ButtonState buttons = GetButtonState();
    if (!game->Update(kTimestep, buttons)) {
      return 0;
    }

    next_frame += 1000 * kTimestep;
    while (SDL_GetTicks() < next_frame) {
      // Draw.
      game->Draw(global_renderer);
      SDL_RenderPresent(global_renderer);
    }
  }

  SDL_DestroyRenderer(global_renderer);
  SDL_DestroyWindow(global_window);
  SDL_Quit();

  return 0;
}