#ifndef BOOKSTORE_2023_LOG_H
#define BOOKSTORE_2023_LOG_H

#include <string>

#include "Tools/File.h"
#include "Tools/String.h"
#include "Tools/UnrolledLinkedList.h"
#include "type_definition.h"

struct FinanceInfo {
  int time_;
  long double income_, outcome_;

  FinanceInfo();
  FinanceInfo(int, const long double &, const long double &);
};
class Log {
public:

  explicit Log(char *, char *, char *, char *);

  void PrintLogFile(std::ostream &);
  void PrintFinanceInfo(std::ostream &, int);
  void PrintFinance(std::ostream &);
  void AddFinanceInfo(FinanceInfo &);
  FinanceInfo GetLastFinanceInfo();
  void AddLogInfo(log_info_t &);
  int GetFinanceFileLength() const;
  void PrintEmployee(std::ostream &);

private:
  static const int logReadLength = 35;
  static const int financeReadLength = 85;

  File finance_file_, log_file_;
  UnrolledLinkedList<user_t, instruction_t> employee_map_;
  int finance_file_length_, log_file_length_;
};

#endif //BOOKSTORE_2023_LOG_H