#include "error.h"

ErrorException::ErrorException(const std::string &message) : message_(message) {}
std::string ErrorException::GetMessage() const {
  return message_;
}