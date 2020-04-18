#include "tilemap.h"

#include <cmath>

const int kTileRows = 16;
const int kTileCols = 16;
const int kTileWidth = 16;
const int kTileHeight = 16;
const Tile kEmptyTile = 0;

const double kBuffer = 0.001;

SDL_Rect ToSDLRect(const Rect& rect) {
  SDL_Rect sr;
  sr.x = rect.x * kTileWidth;
  sr.y = rect.y * kTileHeight;
  sr.w = kTileWidth;
  sr.h = kTileHeight;
  return sr;
}

SDL_Rect TileSet::getTile(Tile tile) const {
  SDL_assert(tile >= 0 && tile < kTileRows*kTileCols);

  int tile_x = tile % kTileCols;
  int tile_y = tile / kTileCols;

  SDL_Rect src;
  src.x = tile_x * kTileWidth;
  src.y = tile_y * kTileHeight;
  src.w = kTileWidth;
  src.h = kTileHeight;

  return src;
}

void TileSet::DrawTile(SDL_Renderer* renderer, Tile tile,
                       const SDL_Rect& dst) const {
  SDL_Rect src = getTile(tile);
  SDL_RenderCopy(renderer, tex, &src, &dst);
}

void TileSet::DrawTileAngle(SDL_Renderer* renderer, Tile tile,
                            const SDL_Rect& dst, double rads) const {
  SDL_Rect src = getTile(tile);
  double degrees = rads * (-180 / 3.14159);
  SDL_RenderCopyEx(renderer, tex, &src, &dst, degrees, NULL, SDL_FLIP_NONE);
}

const char* ToString(TileType type) {
  switch (type) {
    case TileType::NONE: return "NONE";
    case TileType::GROUND: return "GROUND";
    case TileType::SPIKES: return "SPIKES";
    case TileType::OOB: return "OOB";
  }
  SDL_assert(false);
}

TileType TileToTileType(Tile tile) {
  switch(tile) {
    case 0x01:
    case 0x11:
      return TileType::GROUND;
    case 2:
      return TileType::SPIKES;
    default:
      return TileType::NONE;
  }
}

TileMapObjectType TileToTileMapObjectType(Tile tile) {
  switch(tile) {
    case 3:
      return TileMapObjectType::BOX;
    case 4:
      return TileMapObjectType::START;
    case 6:
      return TileMapObjectType::EXIT;
    default:
      return TileMapObjectType::NONE;
  }
}

std::vector<TileMapObject> extractObjects(std::vector<std::vector<Tile>>* map) {
  SDL_assert(map != nullptr);

  std::vector<TileMapObject> objects;

  Rect location;
  location.w = 1;
  location.h = 1;

  for (int row = 0; row < map->size(); ++row) {
    std::vector<Tile>& tilerow = (*map)[row];
    for (int col = 0; col < tilerow.size(); ++col) {
      TileMapObjectType type = TileToTileMapObjectType(tilerow[col]);
      if (type != TileMapObjectType::NONE) {
        // Clear the tile so it shows up empty.
        tilerow[col] = kEmptyTile;

        location.y = row;
        location.x = col;

        TileMapObject object;
        object.type = type;
        object.location = location;
        objects.push_back(object);
      }
    }
  }

  return objects;
}

std::unique_ptr<TileMap> TileMap::Load(
    const std::vector<std::vector<int>>& tiledata, const TileSet* tileset) {
  auto map = std::make_unique<TileMap>();

  map->tileset = tileset;

  // Test to verify that empty tiles parse as NONE.
  SDL_assert(TileToTileType(kEmptyTile) == TileType::NONE);

  // Makes a copy.
  map->map = tiledata;
  // Tiled puts -1 on empty tiles by default.
  for (int y = 0; y < map->map.size(); ++y) {
    for (int x = 0; x < map->map[y].size(); ++x) {
      if (map->map[y][x] < 0) {
        map->map[y][x] = 0;
      }
    }
  }

  map->objects = extractObjects(&map->map);

  return map;
}

std::vector<TileMapObject> TileMap::TileMapObjects() const {
  return objects;
}

void TileMap::Draw(SDL_Renderer* renderer) const {
  SDL_Rect dst;
  dst.w = kTileWidth;
  dst.h = kTileHeight;

  for (int row = 0; row < map.size(); ++row) {
    const std::vector<Tile>& tilerow = map[row];
    for (int col = 0; col < tilerow.size(); ++col) {
      dst.y = row * kTileHeight;
      dst.x = col * kTileWidth;
      tileset->DrawTile(renderer, tilerow[col], dst);
    }
  }
}

TileType TileMap::AtPoint(const Vec& p) const {
  int tile_x = (int)p.x;
  int tile_y = (int)p.y;
  if (tile_y < 0 || tile_y >= map.size()) {
    return TileType::OOB;
  }
  if (tile_x < 0 || tile_x >= map[tile_y].size()) {
    return TileType::OOB;
  }
  return TileToTileType(map[tile_y][tile_x]);
}

std::set<TileType> TileMap::CollidingWith(const Rect& rect) const {
  double x1 = rect.x;
  double y1 = rect.y;
  double x2 = rect.x + rect.w;
  double y2 = rect.y + rect.h;

  return std::set<TileType>({
      AtPoint({x1, y1}),
      AtPoint({x1, y2 - kBuffer}),
      AtPoint({x2 - kBuffer, y1}),
      AtPoint({x2 - kBuffer, y2 - kBuffer}),
  });
}

// HACK: Assume no Rect is larger than a unit square, so just check corners.
CollisionInfo TileMap::XCollide(const Rect& rect, double dx) const {
  double x1 = rect.x + dx;
  double y1 = rect.y;
  double x2 = rect.x + rect.w + dx;
  double y2 = rect.y + rect.h;

  CollisionInfo ci{{}, 0};
  TileType t = TileType::NONE;

  if (dx <= 0) {
    // Velocity is negative, check left side.
    if (t = AtPoint({x1, y1}); t != TileType::NONE) {
      ci.types.insert(t);
      ci.correction = ceil(x1) - x1;
    } 
    if (t = AtPoint({x1, y2 - kBuffer}); t != TileType::NONE) {
      ci.types.insert(t);
      ci.correction = ceil(x1) - x1;
    }
  } else {
    // Velocity is positive, check right side.
    if (t = AtPoint({x2 - kBuffer, y1}); t != TileType::NONE) {
      ci.types.insert(t);
      ci.correction = floor(x2) - x2;
    }
    if (t = AtPoint({x2 - kBuffer, y2 - kBuffer}); t != TileType::NONE) {
      ci.types.insert(t);
      ci.correction = floor(x2) - x2;
    }
  }
  return ci;
}

CollisionInfo TileMap::YCollide(const Rect& rect, double dy) const {
  double x1 = rect.x;
  double y1 = rect.y + dy;
  double x2 = rect.x + rect.w;
  double y2 = rect.y + rect.h + dy;

  CollisionInfo ci{{}, 0};
  TileType t = TileType::NONE;

  if (dy <= 0) {
    // Velocity is negative, check top side;
    if (t = AtPoint({x1, y1}); t != TileType::NONE) {
      ci.types.insert(t);
      ci.correction = ceil(y1) - y1;
    }
    if (t = AtPoint({x2 - kBuffer, y1}); t != TileType::NONE) {
      ci.types.insert(t);
      ci.correction = ceil(y1) - y1;
    }
  } else {
    // Velocity is positive, check bottom side.
    if (t = AtPoint({x1, y2 - kBuffer}); t != TileType::NONE) {
      ci.types.insert(t);
      ci.correction = floor(y2) - y2;
    }
    if (t = AtPoint({x2 - kBuffer, y2 - kBuffer}); t != TileType::NONE) {
      ci.types.insert(t);
      ci.correction = floor(y2) - y2;
    }
  }
  return ci;
}
