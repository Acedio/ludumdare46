#include "level.h"

#include "csvread.h"

std::optional<Level> LoadLevelFromCSV(SDL_Renderer* renderer, const std::string& filename) {
  Level level;

  std::unique_ptr<CSVRead> reader = CSVRead::OpenCSV(filename);
  if (!reader) {
    return std::nullopt;
  }
  level.start.x = reader->ReadInt();
  level.start.y = reader->ReadInt();

  int path_count = reader->ReadInt();
  for (int i = 0; i < path_count; ++i) {
    level.paths.push_back(Path::LoadFromCSV(reader.get()));
  }

  int monster_count = reader->ReadInt();
  for (int i = 0; i < monster_count; ++i) {
    int delay = reader->ReadInt();
    int path = reader->ReadInt();
    std::string anim_file = reader->ReadString();
    SDL_assert(path < level.paths.size());
    Path path_with_delay = level.paths[path];
    path_with_delay.Update(-delay);
    level.monsters.emplace_back(path_with_delay, Animation::LoadFromCSV(renderer, anim_file));
  }

  return level;
}
