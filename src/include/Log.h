#ifndef BOOKSTORE_2023_LOG_H
#define BOOKSTORE_2023_LOG_H

#include <string>

#include "Tools/File.h"
#include "Tools/String.h"
#include "Tools/UnrolledLinkedList.h"

class Log {
public:
  using employee_t = String<65>;
  using instruction_t = String<280>;

  Log(char *, char *, char *, char *);

  void PrintLogFile();
  void PrintFinanceInfo(int);
  void PrintFinance();

  int finance_file_length;

private:
  instruction_t GetLogInfo();

  File finance_file_, log_file_;
  UnrolledLinkedList<employee_t, instruction_t> employee_map_;
};

#endif //BOOKSTORE_2023_LOG_H
