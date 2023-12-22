#ifndef BOOKSTORE_2023_LOG_H
#define BOOKSTORE_2023_LOG_H

#include <string>

#include "Tools/File.h"
#include "Tools/String.h"
#include "Tools/UnrolledLinkedList.h"

struct FinanceInfo {
  int time_;
  long double income_, outcome_;

  FinanceInfo();
  FinanceInfo(int, const long double &, const long double &);
};
class Log {
public:
  using employee_t = String<65>;
  using instruction_t = String<280>;
  using log_info_t = String<350>;

  Log(char *, char *, char *, char *);

  void PrintLogFile(std::ostream &);
  void PrintFinanceInfo(std::ostream &, int);
  void PrintFinance(std::ostream &);
  void AddFinanceInfo(FinanceInfo &);
  void AddLogInfo(log_info_t &);
  int GetFinanceFileLength() const;

private:
  static const int logReadLength = 35;
  static const int financeReadlength = 85;

  File finance_file_, log_file_;
  UnrolledLinkedList<employee_t, instruction_t> employee_map_;
  int finance_file_length_, log_file_length_;
};

#endif //BOOKSTORE_2023_LOG_H