#include "level.h"

#include "csvread.h"

std::unique_ptr<Level> LoadLevelFromCSV(SDL_Renderer* renderer, const std::string& filename) {
  std::unique_ptr<CSVRead> reader = CSVRead::OpenCSV(filename);
  if (!reader) {
    return nullptr;
  }
  std::unique_ptr<Level> level = std::make_unique<Level>();

  level->start.x = reader->ReadInt();
  level->start.y = reader->ReadInt();

  int path_count = reader->ReadInt();
  std::vector<Path> paths;
  for (int i = 0; i < path_count; ++i) {
    paths.push_back(Path::LoadFromCSV(reader.get()));
  }

  int monster_count = reader->ReadInt();
  for (int i = 0; i < monster_count; ++i) {
    int delay = reader->ReadInt();
    int path = reader->ReadInt();
    std::string monster_file = reader->ReadString();
    SDL_assert(path < paths.size());
    Path path_with_delay = paths[path];
    path_with_delay.Update(-delay);
    level->monsters.push_back(Monster::LoadFromCSV(renderer, path_with_delay, monster_file));
  }

  return level;
}
