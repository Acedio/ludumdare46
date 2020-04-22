#include "tilemap.h"

#include <cmath>
#include <fstream>
#include <string>

#include "transform.h"

const int kTileRows = 32;
const int kTileCols = 32;
const Tile kEmptyTile = 0;

const double kBuffer = 0.001;

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

void TileSet::DrawTile(SDL_Renderer* renderer, const Camera& camera, Tile tile,
                       const SDL_Rect& dst) const {
  SDL_Rect src = getTile(tile);
  SDL_Rect transformed = camera.Transform(dst);
  SDL_RenderCopy(renderer, tex, &src, &transformed);
}

void TileSet::DrawTileAngle(SDL_Renderer* renderer, const Camera& camera, Tile tile,
                            const SDL_Rect& dst, double rads) const {
  SDL_Rect src = getTile(tile);
  SDL_Rect transformed = camera.Transform(dst);
  double degrees = rads * (-180 / 3.14159);
  SDL_RenderCopyEx(renderer, tex, &src, &transformed, degrees, NULL, SDL_FLIP_NONE);
}

const char* ToString(TileType type) {
  switch (type) {
    case TileType::NONE: return "NONE";
    case TileType::BLOCKED: return "BLOCKED";
    case TileType::OOB: return "OOB";
  }
  SDL_assert(false);
}

TileType TileToTileType(Tile tile) {
  switch(tile) {
    case 1:
      return TileType::BLOCKED;
    default:
      return TileType::NONE;
  }
}

std::vector<std::vector<int>> TilesFromCSV(const std::string& filename) {
  std::ifstream map_file;
  map_file.open(filename, std::ifstream::in);

  std::vector<std::vector<int>> rows;

  while (map_file.good()) {
    std::string line;
    std::getline(map_file, line);
    int c0 = 0;
    std::vector<Tile> row;
    for (int i = 0; i < line.length(); ++i) {
      if (line[i] == ',') {
        std::string tile_str = line.substr(c0, i - c0);
        int tile = std::stoi(tile_str);
        row.push_back(tile);
        c0 = i + 1; // skip the comma
      }
    }
    if (row.size() == 0) {
      // There were no tiles in this row, stop parsing.
      break;
    }
    // Parse the last tile in the row (no comma at line end).
    int tile = std::stoi(line.substr(c0));
    row.push_back(tile);
    rows.push_back(row);
  }

  return rows;
}

std::unique_ptr<TileMap> TileMap::LoadLayersFromCSVs(
  const std::string& filename, const TileSet* tileset) {
  auto map = std::make_unique<TileMap>();

  map->tileset = tileset;

  // Test to verify that empty tiles parse as NONE.
  SDL_assert(TileToTileType(kEmptyTile) == TileType::NONE);

  // Makes a copy.
  map->back = TilesFromCSV(filename + "_back.csv");
  map->front = TilesFromCSV(filename + "_front.csv");
  map->collision = TilesFromCSV(filename + "_collision.csv");

  return map;
}

void TileMap::DrawTiles(SDL_Renderer* renderer, const Camera& camera, const std::vector<std::vector<Tile>>& tiles) const {
  SDL_Rect dst;
  dst.w = kTileWidth;
  dst.h = kTileHeight;

  for (int row = 0; row < tiles.size(); ++row) {
    const std::vector<Tile>& tilerow = tiles[row];
    for (int col = 0; col < tilerow.size(); ++col) {
      dst.y = row * kTileHeight;
      dst.x = col * kTileWidth;
      if (tilerow[col] >= 0) {
        tileset->DrawTile(renderer, camera, tilerow[col], dst);
      }
    }
  }
}

void TileMap::DrawBackground(SDL_Renderer* renderer, const Camera& camera) const {
  DrawTiles(renderer, camera, back);
}

void TileMap::DrawForeground(SDL_Renderer* renderer, const Camera& camera) const {
  DrawTiles(renderer, camera, front);
}

TileType TileMap::AtPoint(const Vec& p) const {
  int tile_x = (int)p.x;
  int tile_y = (int)p.y;
  if (tile_y < 0 || tile_y >= collision.size()) {
    return TileType::OOB;
  }
  if (tile_x < 0 || tile_x >= collision[tile_y].size()) {
    return TileType::OOB;
  }
  return TileToTileType(collision[tile_y][tile_x]);
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

SDL_Rect TileMap::Bounds() {
  SDL_assert(back.size() > 0);
  SDL_Rect bounds;
  bounds.x = 0;
  bounds.y = 0;
  bounds.w = back[0].size() * kTileHeight;
  bounds.h = back.size() * kTileWidth;
  return bounds;
}
