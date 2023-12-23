#include "read_string.h"
#include "error.h"

bool IsTokenEnd(std::istream &is){
  if (!is.good()) {
    return true;
  }
  char c = is.get();
  if (c == EOF || c == ' ' || c == '\r' || c == '\n') {
    is.unget();
    return true;
  }
  return false;
}
void SkipSpaces(std::istream &is) {
  if (!is.good()) {
    return;
  }
  char c;
  while (true) {
    c = is.get();
    if (c == EOF || c != ' ') {
      is.unget();
      return;
    }
  }
}
bool ReadLine(std::istream &is) {
  if (!is.good()) {
    return false;
  }
  char c;
  bool has_char = false;
  while (true) {
    c = is.get();
    if (c == EOF || c == '\n') {
      break;
    }
    has_char = (c != ' ' && c != '\r');
  }
  return has_char;
}
bool IsEndOfLine(std::istream &is) {
  if (!is.good()) {
    return true;
  }
  char c = is.peek();
  return c == EOF || c == '\n' || c == '\r';
}
void ReadFormat(std::istream &is, const std::string &format) {
  if (!is.good()) {
    throw ErrorException("INVALID INPUT FORMAT");
  }
  int i;
  char c;
  for (i = 0; i < format.length(); i++) {
    c = is.get();
    if (c == EOF || c != format[i]) {
      is.unget();
      throw ErrorException("INVALID INPUT FORMAT");
    }
  }
}
std::string ReadString(std::istream &is, bool number, bool letter, bool underline, bool quotes, bool other_print,
                       bool is_end_of_line, bool have_quotes, bool end, bool could_be_empty) {
  if (!is.good()) {
    throw ErrorException("INVALID INPUT FORMAT");
  }
  std::string result;
  char c;
  if (have_quotes) {
    ReadFormat(is, "\"");
  }
  auto IsCharValid = [number, letter, underline, quotes, other_print](char c)->bool {
    if (c == ' ') {
      return false;
    }
    if (isdigit(c)) {
      return number;
    }
    if (isalpha(c)) {
      return letter;
    }
    if (c == '_') {
      return underline;
    }
    if (c == '\"') {
      return quotes;
    }
    if (isprint(c)) {
      return other_print;
    }
    return false;
  };
  while (true) {
    c = is.get();
    if (c == EOF || c == '\r' || c == '\n') {
      is.unget();
      break;
    }
    if (IsCharValid(c)) {
      result.push_back(c);
    }
    else {
      is.unget();
      break;
    }
  }
  if (!could_be_empty && result.empty()) {
    throw ErrorException("INVALID INPUT FORMAT");
  }
  if (have_quotes) {
    ReadFormat(is, "\"");
  }
  if (end) {
    if (!IsTokenEnd(is)) {
      throw ErrorException("INVALID INPUT FORMAT");
    }
    SkipSpaces(is);
  }
  if (is_end_of_line) {
    if (!IsEndOfLine(is)) {
      throw ErrorException("INVALID INPUT FORMAT");
    }
    ReadLine(is);
  }
  return result;
}
std::string ReadDouble(std::istream &is, bool is_end_of_line) {
  if (!is.good()) {
    throw ErrorException("INVALID INPUT FORMAT");
  }
  std::string result(ReadString(is, true, false, false, false, false, false, false, false, true));
  if (IsTokenEnd(is)) {
    return result;
  }
  if (is.get() != '.') {
    is.unget();
    throw ErrorException("INVALID INPUT FORMAT");
  }
  result.push_back('.');
  std::string decimal(ReadString(is, true, false, false, false, false, is_end_of_line, false, true, true));
  if (result == "." && decimal.empty()) {
    throw ErrorException("INVALID INPUT FORMAT");
  }
  result += decimal;
  return result;
}
std::vector<std::string> ParseKeywords(const std::string &keywords) {
  std::vector<std::string> result;
  if (keywords.empty()) {
    return result;
  }
  if (keywords.back() == '|') {
    throw ErrorException("INVALID INPUT FORMAT");
  }
  int i = 0;
  while (i < keywords.length()) {
    std::string word;
    while (i < keywords.length() && keywords[i] != '|') {
      word.push_back(keywords[i]);
      i++;
    }
    i++;
    if (word.empty()) {
      throw ErrorException("INVALID INPUT FORMAT");
    }
    for (const auto &prev_word : result) {
      if (prev_word == word) {
        throw ErrorException("INVALID INPUT FORMAT");
      }
    }
    result.push_back(word);
  }
  return result;
}