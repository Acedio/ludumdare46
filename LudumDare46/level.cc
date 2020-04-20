#include "level.h"

#include "csvread.h"

std::optional<Level> LoadLevelFromCSV(const std::string& filename) {
  Level level;

  std::unique_ptr<CSVRead> reader = CSVRead::OpenCSV(filename);
  if (!reader) {
    return std::nullopt;
  }
  level.start.x = reader->ReadInt();
  level.start.y = reader->ReadInt();

  return level;
}
