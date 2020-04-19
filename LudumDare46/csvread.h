#ifndef CSVREAD_H
#define CSVREAD_H

#include <fstream>

class CSVRead {
public:
  static std::unique_ptr<CSVRead> OpenCSV(const std::string& filename);

  std::string ReadString();
  int ReadInt();

private:
  CSVRead(std::unique_ptr<std::istream> stream) : in(std::move(stream)) {}

  bool isDelim(char c) {
    return c == ' ' || c == ',' || c == '\n';
  }

  std::unique_ptr<std::istream> in;
};

#endif // CSVREAD_H
