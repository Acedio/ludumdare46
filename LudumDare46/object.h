#ifndef EXIT_H
#define EXIT_H

#include <optional>
#include <vector>
#include <SDL.h>

#include "event.h"
#include "geometry.h"
#include "sprite.h"
#include "tilemap.h"

enum class ObjectType {
  NONE,
  EXIT,
};

struct Object {
  ObjectType type = ObjectType::NONE;
  Rect bounding_box;
  std::optional<Sprite> sprite;
};

class ObjectManager {
 public:
  ObjectManager(const TileSet* tileset) : tileset(tileset) {}

  void AddTileMapObject(TileMapObjectType type, Vec pos);

  std::vector<Event> Update(double t, const Rect& hero_box);
  void Draw(SDL_Renderer* renderer) const;

 private:
  std::vector<Object> objects;

  // Not owned.
  const TileSet* tileset;
};

#endif
