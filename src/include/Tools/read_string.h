#ifndef BOOKSTORE_2023_READ_STRING_H
#define BOOKSTORE_2023_READ_STRING_H

#include <string>
#include <iostream>
#include <vector>

bool IsTokenEnd(std::istream &);
void SkipSpaces(std::istream &);
bool ReadLine(std::istream &);
bool IsEndOfLine(std::istream &);
void ReadFormat(std::istream &, const std::string &);
std::string ReadString(std::istream &, bool, bool, bool, bool, bool, bool, bool end = true, bool could_be_empty = false,
                       bool is_end_of_line = false);
std::string ReadDouble(std::istream &, bool is_end_of_line = false); // precision >= 1
std::vector<std::string> ParseKeywords(const std::string &);

#endif //BOOKSTORE_2023_READ_STRING_H