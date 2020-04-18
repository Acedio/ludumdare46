#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <string>

void log(const std::string& text) {
  std::cout << text << std::endl;
}

void check(bool val, const std::string& text) {
  if (!val) {
    log(text);
  }
}

#endif
