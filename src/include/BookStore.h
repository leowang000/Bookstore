#ifndef BOOKSTORE_2023_BOOKSTORE_H
#define BOOKSTORE_2023_BOOKSTORE_H

#include "Instruction.h"
#include "Accounts.h"
#include "Books.h"
#include "Log.h"
#include "type_definition.h"
#include "Tools/read_string.h"

class BookStore {
public:
  explicit BookStore(char *, char *, char *, char *, char *, char *, char *, char *, char *, char *, char *, char *, char *,
            char *, char *);

  void Init();
  void GetInstruction(const std::string &);
  void CheckInstruction();
  bool ExecuteInstruction();
  void AddLog();

private:
  int time_;
  bool is_instruction_valid_;
  Accounts accounts_;
  Books books_;
  Log log_;
  Instruction *inst_;
};

#endif //BOOKSTORE_2023_BOOKSTORE_H