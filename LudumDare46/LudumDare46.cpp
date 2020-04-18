// LudumDare46.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <SDL.h>

int main(int argc, char **argv) {
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
    return -1;
  }

  SDL_Window *global_window = SDL_CreateWindow("Ludum Dare 46!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, 0);
  if (!global_window) {
    return -1;
  }
  SDL_Renderer *global_renderer = SDL_CreateRenderer(global_window, -1, 0);
  if (!global_renderer) {
    return -1;
  }
  SDL_RenderSetScale(global_renderer, 2, 2);
  SDL_SetRenderDrawColor(global_renderer, 255, 0, 255, 255);
  SDL_RenderClear(global_renderer);
  SDL_RenderPresent(global_renderer);

  std::cout << "Hello!" << std::endl;
  SDL_Delay(5000);

  SDL_DestroyRenderer(global_renderer);
  SDL_DestroyWindow(global_window);
  SDL_Quit();

  return 0;
}