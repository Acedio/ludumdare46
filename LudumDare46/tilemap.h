#ifndef TILEMAP_H
#define TILEMAP_H

#include <memory>
#include <set>
#include <vector>

#include <SDL.h>

#include "geometry.h"

SDL_Rect ToSDLRect(const Rect& rect);

typedef int Tile;

class TileSet {
 public:
  TileSet(SDL_Texture* tex) : tex(tex) {}
  void DrawTile(SDL_Renderer* renderer, Tile tile,
                const SDL_Rect& dst) const;
  void DrawTileAngle(SDL_Renderer* renderer, Tile tile, const SDL_Rect& dst,
                     double rads) const;

 private:
  SDL_Rect getTile(Tile tile) const;
  // Assumes 16x16 tilemap.
  SDL_Texture* tex;
};

enum class TileType {
  NONE,
  GROUND,
  SPIKES,
  OOB
};

const char* ToString(TileType type);
TileType TileToTileType(Tile tile);

enum class TileMapObjectType {
  NONE,
  BOX,
  START,
  EXIT
};

struct TileMapObject {
  TileMapObjectType type;
  // The coordinates and size of the object in tilespace.
  Rect location;
};

struct CollisionInfo {
  std::set<TileType> types;

  // A magnitude the colliding rect could move to not collide with the TileMap
  // anymore.
  double correction;
};

class TileMap {
 public:
  // Draw the map, no viewport.
  void Draw(SDL_Renderer* renderer) const;

  std::vector<TileMapObject> TileMapObjects() const;

  std::set<TileType> CollidingWith(const Rect& rect) const;
  // TODO: Maybe pass a set of TileType to indicate what the rect cares about?
  CollisionInfo XCollide(const Rect& rect, double dx) const;
  CollisionInfo YCollide(const Rect& rect, double dy) const;

  TileType AtPoint(const Vec& p) const;

  static std::unique_ptr<TileMap> Load(
      const std::vector<std::vector<int>>& tiledata, const TileSet* tileset);

 private:
  std::vector<std::vector<Tile>> map;
  const TileSet* tileset;
  std::vector<TileMapObject> objects;
};

#endif
