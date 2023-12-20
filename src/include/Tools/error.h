#ifndef BOOKSTORE_2023_ERROR_H
#define BOOKSTORE_2023_ERROR_H

#include <exception>
#include <string>

class ErrorException : public std::exception {
public:
  explicit ErrorException(const std::string &message);

  std::string GetMessage() const;

private:
  std::string message_;
};

#endif //BOOKSTORE_2023_ERROR_H
