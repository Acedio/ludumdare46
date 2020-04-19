#include "csvread.h"

#include <string>

std::unique_ptr<CSVRead> CSVRead::OpenCSV(const std::string& filename) {
  std::unique_ptr<std::ifstream> csv_stream(new std::ifstream);
  csv_stream->open(filename, std::ifstream::in);
  if (!csv_stream->good()) {
    return nullptr;
  }
  return std::unique_ptr<CSVRead>(new CSVRead(std::move(csv_stream)));
}

std::string CSVRead::ReadString() {
  if (!in->good()) {
    return "";
  }

  while (in->good()) {
    char c = in->peek();
    if (isDelim(c)) {
      in->get();
    } else {
      break;
    }
  }

  std::string str;
  while (in->good()) {
    char c = in->get();
    if (isDelim(c)) {
      break;
    } else {
      str.append(&c, 1);
    }
  }
  return str;
}

int CSVRead::ReadInt() {
  return std::stoi(ReadString());
}
