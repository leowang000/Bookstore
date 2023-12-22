#ifndef BOOKSTORE_2023_BOOKSTORE_H
#define BOOKSTORE_2023_BOOKSTORE_H

#include "Instruction.h"
#include "Accounts.h"
#include "Books.h"
#include "Log.h"
#include "type_definition.h"

class Instruction;
class Accounts;
class Books;
class Log;
class BookStore {
public:
  friend Instruction;
  friend Accounts;
  friend Books;
  friend Log;

  explicit BookStore(char *, char *, char *, char *, char *, char *, char *, char *, char *, char *, char *, char *, char *,
            char *, char *);
  ~BookStore();

  void GetInstruction();
  void ExecuteInstruction();

private:
  Accounts accounts_;
  Books books_;
  Log log_;
  Instruction *inst_;

  void GetLogInfo();
};

#endif //BOOKSTORE_2023_BOOKSTORE_H
