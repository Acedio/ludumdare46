#ifndef TILEMAP_H
#define TILEMAP_H

#include <memory>
#include <set>
#include <vector>

#include <SDL.h>

#include "camera.h"
#include "geometry.h"

typedef int Tile;

// TODO: Rename this and share this with Animations.
class TileSet {
 public:
  TileSet(SDL_Texture* tex) : tex(tex) {}
  void DrawTile(SDL_Renderer* renderer, const Camera& camera, Tile tile,
                const SDL_Rect& dst) const;
  void DrawTileAngle(SDL_Renderer* renderer, const Camera& camera, Tile tile, const SDL_Rect& dst,
                     double rads) const;

 private:
  // Assumes 32x32 tilemap.
  SDL_Rect getTile(Tile tile) const;
  SDL_Texture* tex;
};

enum class TileType {
  NONE,
  BLOCKED,
  OOB
};

const char* ToString(TileType type);
TileType TileToTileType(Tile tile);

struct CollisionInfo {
  std::set<TileType> types;

  // A magnitude the colliding rect could move to not collide with the TileMap
  // anymore.
  double correction;
};

class TileMap {
 public:
  // Draw the map, no viewport.
  void DrawBackground(SDL_Renderer* renderer, const Camera& camera) const;
  void DrawForeground(SDL_Renderer* renderer, const Camera& camera) const;

  std::set<TileType> CollidingWith(const Rect& rect) const;
  // TODO: Maybe pass a set of TileType to indicate what the rect cares about?
  CollisionInfo XCollide(const Rect& rect, double dx) const;
  CollisionInfo YCollide(const Rect& rect, double dy) const;

  TileType AtPoint(const Vec& p) const;

  SDL_Rect Bounds();

  static std::unique_ptr<TileMap> LoadLayersFromCSVs(
    const std::string& file_prefix, const TileSet* tileset);

 private:
  void DrawTiles(SDL_Renderer* renderer, const Camera& camera, const std::vector<std::vector<Tile>>& tiles) const;

  std::vector<std::vector<Tile>> front;
  std::vector<std::vector<Tile>> back;
  std::vector<std::vector<Tile>> collision;
  const TileSet* tileset;
};

#endif
