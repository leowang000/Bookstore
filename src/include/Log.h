#ifndef BOOKSTORE_2023_LOG_H
#define BOOKSTORE_2023_LOG_H

#include <string>

#include "Tools/File.h"
#include "Tools/String.h"
#include "Tools/UnrolledLinkedList.h"

class Log {
public:
  Log(char *, char *, char *, char *);

private:
  UnrolledLinkedList<String<65>, String<280>> employee_map_;
  File finance_file_, log_file_;

  String<280> GetLogInfo();
};

#endif //BOOKSTORE_2023_LOG_H
