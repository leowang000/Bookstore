#ifndef BOOKSTORE_2023_STRING_H
#define BOOKSTORE_2023_STRING_H

#include <iostream>
#include <cstring>
#include <sstream>
#include <iomanip>

#include "error.h"

template<int len> class String;
template<int len> std::ostream &operator<<(std::ostream &, const String<len> &);
template<int len> std::istream &operator>>(std::istream &, String<len> &);
template<int len>
class String {
public:
  String();
  String(const String<len> &);
  String(const char *);
  String(const std::string &);
  explicit String(long long, bool left = true);
  explicit String(const long double &, int);
  String &operator=(const String<len> &);

  char &operator[](int);
  bool operator<(const String<len> &) const;
  bool operator>(const String<len> &) const;
  bool operator<=(const String<len> &) const;
  bool operator==(const String<len> &) const;
  bool operator>=(const String<len> &) const;
  bool operator!=(const String<len> &) const;
  friend std::ostream &operator<<<len>(std::ostream &, const String<len> &);
  friend std::istream &operator>><len>(std::istream &, String<len> &);
  int ToInt(int left = true) const;
  long long ToLongLong() const;
  long double ToDouble(int) const;
  std::string GetString(int precision = -1, bool fill = true) const;
  bool Empty() const;

private:
  char str_[len + 1];
};

template<int len>
String<len>::String() {
  int i;
  for (i = 0; i <= len; i++) {
    str_[i] = '\0';
  }
}
template<int len>
String<len>::String(const String<len> &other) {
  strncpy(str_, other.str_, len + 1);
  int i;
  for (i = strlen(other.str_); i <= len; i++) {
    str_[i] = '\0';
  }
}
template<int len>
String<len>::String(const char *other) {
  if (strlen(other) > len) {
    throw ErrorException("INVALID INPUT FORMAT");
  }
  int i;
  strcpy(str_, other);
  for (i = strlen(other); i <= len; i++) {
    str_[i] = '\0';
  }
}
template<int len>
String<len>::String(const std::string &other) : String(other.c_str()) {}
template<int len>
String<len>::String(long long other, bool left) {
  std::stringstream str;
  int i;
  str << other;
  if (left) {
    strcpy(str_, str.str().c_str());
  }
  else {
    for (i = 0; i < len - str.str().length(); i++) {
      str_[i] = ' ';
    }
    strcpy(str_ + len - str.str().length(), str.str().c_str());
  }
}
template<int len>
String<len>::String(const long double &other, int precision) {
  std::stringstream str;
  str << std::fixed << std::setprecision(precision) << other;
  strcpy(str_, str.str().c_str());
}
template<int len>
String<len> &String<len>::operator=(const String<len> &other) {
  if (this == &other) {
    return *this;
  }
  strncpy(str_, other.str_, len + 1);
  int i;
  for (i = strlen(other.str_); i <= len; i++) {
    str_[i] = '\0';
  }
  return *this;
}
template<int len>
char &String<len>::operator[](int index) {
  if (index < 0 || index >= len) {
    throw ErrorException("INDEX OUT OF RANGE");
  }
  return str_[index];
}
template<int len>
bool String<len>::operator<(const String<len> &rhs) const {
  return strncmp(str_, rhs.str_, len + 1) < 0;
}
template<int len>
bool String<len>::operator>(const String<len> &rhs) const {
  return strncmp(str_, rhs.str_, len + 1) > 0;
}
template<int len>
bool String<len>::operator<=(const String<len> &rhs) const {
  return strncmp(str_, rhs.str_, len + 1) <= 0;
}
template<int len>
bool String<len>::operator==(const String<len> &rhs) const {
  return strncmp(str_, rhs.str_, len + 1) == 0;
}
template<int len>
bool String<len>::operator>=(const String<len> &rhs) const {
  return strncmp(str_, rhs.str_, len + 1) >= 0;
}
template<int len>
bool String<len>::operator!=(const String<len> &rhs) const {
  return strncmp(str_, rhs.str_, len + 1) != 0;
}
template<int len>
std::ostream &operator<<(std::ostream &os, const String<len> &str) {
  os << str.GetString();
  return os;
}
template<int len>
std::istream &operator>>(std::istream &is, String<len> &str) {
  is >> str.str_;
  return is;
}
template<int len>
int String<len>::ToInt(int left) const {
  int i, result = 0;
  if (left) {
    for (i = 0; str_[i] != '\0'; i++) {
      result = result * 10 + str_[i] - '0';
    }
  }
  else {
    int base = 1;
    for (i = len - 1; isdigit(str_[i]); i--) {
      result += base * (str_[i] - '0');
      base *= 10;
    }
  }
  return result;
}
template<int len>
long long String<len>::ToLongLong() const {
  int i;
  long long result = 0;
  for (i = 0; str_[i] != '\0'; i++) {
    result = result * 10 + str_[i] - '0';
  }
  return result;
}
template<int len>
long double String<len>::ToDouble(int precision) const {
  int i, j;
  double result = 0, base = 1;
  for (i = 0; str_[i] != '.' && i < strlen(str_); i++) {
    result = result * 10 + str_[i] - '0';
  }
  if (i == strlen(str_)) {
    return result;
  }
  for (j = i + 1; str_[j] != '\0' && j <= i + precision; j++) {
    base /= 10;
    result += base * (str_[j] - '0');
  }
  return result;
}
template<int len>
std::string String<len>::GetString(int precision, bool fill) const {
  if (precision == -1) {
    std::string result(str_);
    if (fill) {
      while (result.length() < len) {
        result.push_back(' ');
      }
    }
    return result;
  }
  std::stringstream res;
  if (precision == 0) {
    res << std::fixed << std::setprecision(precision) << ToInt();
  }
  else {
    res << std::fixed << std::setprecision(precision) << ToDouble(precision);
  }
  return res.str();
}
template<int len>
bool String<len>::Empty() const {
  return str_[0] == '\0';
}

#endif //BOOKSTORE_2023_STRING_H