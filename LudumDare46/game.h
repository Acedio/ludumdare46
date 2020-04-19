#ifndef GAME_H
#define GAME_H

#include <SDL_mixer.h>

#include "animation.h"
#include "box.h"
#include "buttons.h"
#include "camera.h"
#include "geometry.h"
#include "hero.h"
#include "monster.h"
#include "object.h"
#include "particle.h"
#include "tilemap.h"

class Game {
 public:
  ~Game();

  // Returns false if the game has ended.
  bool Update(SDL_Renderer* renderer, ButtonState buttons, double t);
  void Draw(SDL_Renderer* renderer) const;

  static std::unique_ptr<Game> Load(SDL_Renderer* renderer);
 private:
  Game() : particles(Rect{-10, -10, 100, 100}) {}

  void LoadLevel(SDL_Renderer *renderer, int level, const TileSet* tileset);

  std::unique_ptr<TileSet> tileset;
  // Owned.
  SDL_Texture* tileset_texture;
  SDL_Texture* overlay_texture;
  Mix_Chunk* jump_sound;
  Mix_Chunk* hurt_sound;

  void InitializeSound();

  // TODO: Update with actual screen coords.
  ParticleManager particles;

  int level = 0;
  bool audio_initialized = false;
  bool audio_error = true;

  // These are reloaded with each level load.
  std::unique_ptr<TileMap> tilemap;
  std::unique_ptr<Monster> monster;
  std::unique_ptr<Hero> hero;
  std::unique_ptr<BoxManager> boxes;
  std::unique_ptr<ObjectManager> objects;
  std::unique_ptr<Camera> camera;
};

#endif
