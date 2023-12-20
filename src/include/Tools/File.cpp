#include <fstream>

#include "File.h"

File::File(char *file_name) : file_name_(file_name) {}
bool File::Open() {
  this->open(file_name_);
  if (good()) {
    return true;
  }
  this->open(file_name_, std::ios::out);
  this->close();
  this->open(file_name_);
  return false;
}