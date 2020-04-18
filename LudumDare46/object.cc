#include "object.h"

std::vector<Event> ObjectManager::Update(double t, const Rect& hero_box) {
  std::vector<Event> events;
  for (const Object& object : objects) {
    if (!Intersects(object.bounding_box, hero_box)) {
      continue;
    }
    switch (object.type) {
      case ObjectType::EXIT:
        events.push_back(Event{EventType::WIN});
        break;
      default:
        break;
    }
  }
  return events;
}

void ObjectManager::Draw(SDL_Renderer* renderer) const {
  for (const Object& object : objects) {
    SDL_Rect dst = ToSDLRect(object.bounding_box);
    if (object.sprite) {
      object.sprite->Draw(renderer, dst);
    }
  }
}

ObjectType ToObjectType(TileMapObjectType type) {
  switch (type) {
    case TileMapObjectType::EXIT:
      return ObjectType::EXIT;
    default:
      SDL_assert(false);
      return ObjectType::NONE;
  }
}

void ObjectManager::AddTileMapObject(TileMapObjectType type, Vec pos) {
  Object object;
  object.type = ToObjectType(type);
  object.bounding_box = {pos.x, pos.y, 1, 1};
  // TODO: Make this dynamic.
  object.sprite = Sprite(tileset, 6);
  objects.push_back(object);
}
