#ifndef BOOKSTORE_2023_INSTRUCTION_H
#define BOOKSTORE_2023_INSTRUCTION_H

#include <string>

#include "Accounts.h"
#include "Books.h"
#include "Log.h"
#include "type_definition.h"

class Instruction {
public:
  explicit Instruction(int);

  virtual void Execute(Accounts &, Books &, Log &);
  virtual void CheckParameter(Accounts &, Books &, Log &);
  virtual void CheckPrivilege(Accounts &);
  virtual instruction_t GetString() const;

protected:
  system_time_t time_;
};
class QuitInst : public Instruction {
public:
  static const int instructionPrivilege = 0;

  explicit QuitInst(int, const std::string &);

  void Execute(Accounts &, Books &, Log &);
  void CheckParameter(Accounts &, Books &, Log &);
  void CheckPrivilege(Accounts &);
  instruction_t GetString() const;

private:
  bool is_quit;
};
class SuInst : public Instruction {
public:
  static const int instructionPrivilege = 0;

  explicit SuInst(int, const std::string &, const std::string &);

  void Execute(Accounts &, Books &, Log &);
  void CheckParameter(Accounts &, Books &, Log &);
  void CheckPrivilege(Accounts &);
  instruction_t GetString() const;

private:
  user_id_t user_id_;
  password_t password_;
};
class LogoutInst : public Instruction {
public:
  static const int instructionPrivilege = 1;

  explicit LogoutInst(int);

  void Execute(Accounts &, Books &, Log &);
  void CheckParameter(Accounts &, Books &, Log &);
  void CheckPrivilege(Accounts &);
  instruction_t GetString() const;
};
class RegisterInst : public Instruction {
public:
  static const int instructionPrivilege = 0;

  explicit RegisterInst(int, const user_id_t &, const password_t &, const username_t &);

  void Execute(Accounts &, Books &, Log &);
  void CheckParameter(Accounts &, Books &, Log &);
  void CheckPrivilege(Accounts &);
  instruction_t GetString() const;

private:
  user_id_t user_id_;
  password_t password_;
  username_t username_;
};
class PasswdInst : public Instruction {
public:
  static const int instructionPrivilege = 1;

  explicit PasswdInst(int, const user_id_t &, const password_t &, const password_t &);

  void Execute(Accounts &, Books &, Log &);
  void CheckParameter(Accounts &, Books &, Log &);
  void CheckPrivilege(Accounts &);
  instruction_t GetString() const;

private:
  user_id_t user_id_;
  password_t current_password_, new_password_;
};
class UserAddInst : public Instruction {
public:
  static const int instructionPrivilege = 3;

  explicit UserAddInst(int, const user_id_t &, const password_t &, const privilege_t &, const username_t &);

  void Execute(Accounts &, Books &, Log &);
  void CheckParameter(Accounts &, Books &, Log &);
  void CheckPrivilege(Accounts &);
  instruction_t GetString() const;

private:
  user_id_t user_id_;
  password_t password_;
  privilege_t privilege_;
  username_t username_;
};
class DeleteInst : public Instruction {
public:
  static const int instructionPrivilege = 7;

  explicit DeleteInst(int, const user_id_t &);

  void Execute(Accounts &, Books &, Log &);
  void CheckParameter(Accounts &, Books &, Log &);
  void CheckPrivilege(Accounts &);
  instruction_t GetString() const;

private:
  user_id_t user_id_;
};
class ShowInst : public Instruction {
public:
  static const int instructionPrivilege = 1;

  explicit ShowInst(int, const Books::Book &);

  void Execute(Accounts &, Books &, Log &);
  void CheckParameter(Accounts &, Books &, Log &);
  void CheckPrivilege(Accounts &);
  instruction_t GetString() const;

private:
  Books::Book search_index_;
};
class BuyInst : public Instruction {
public:
  static const int instructionPrivilege = 1;

  explicit BuyInst(int, const ISBN_t &, const quantity_t &);

  void Execute(Accounts &, Books &, Log &);
  void CheckParameter(Accounts &, Books &, Log &);
  void CheckPrivilege(Accounts &);
  instruction_t GetString() const;

private:
  ISBN_t ISBN_;
  quantity_t quantity_;
};
class SelectInst : public Instruction {
public:
  static const int instructionPrivilege = 3;

  explicit SelectInst(int, const ISBN_t &);

  void Execute(Accounts &, Books &, Log &);
  void CheckParameter(Accounts &, Books &, Log &);
  void CheckPrivilege(Accounts &);
  instruction_t GetString() const;

private:
  ISBN_t ISBN_;
};
class ModifyInst : public Instruction {
public:
  static const int instructionPrivilege = 3;

  explicit ModifyInst(int, const Books::Book &);

  void Execute(Accounts &, Books &, Log &);
  void CheckParameter(Accounts &, Books &, Log &);
  void CheckPrivilege(Accounts &);
  instruction_t GetString() const;

private:
  Books::Book modification_;
};
class ImportInst : public Instruction {
public:
  static const int instructionPrivilege = 3;

  explicit ImportInst(int, const quantity_t &, const price_t &);

  void Execute(Accounts &, Books &, Log &);
  void CheckParameter(Accounts &, Books &, Log &);
  void CheckPrivilege(Accounts &);
  instruction_t GetString() const;

private:
  quantity_t quantity_;
  price_t cost_;
};
class ShowFinanceInst : public Instruction {
public:
  static const int instructionPrivilege = 7;

  explicit ShowFinanceInst(int, const count_t &);

  void Execute(Accounts &, Books &, Log &);
  void CheckParameter(Accounts &, Books &, Log &);
  void CheckPrivilege(Accounts &);
  instruction_t GetString() const;

private:
  count_t count_;
};
class ReportFinanceInst : public Instruction {
public:
  static const int instructionPrivilege = 7;

  explicit ReportFinanceInst(int);

  void Execute(Accounts &, Books &, Log &);
  void CheckParameter(Accounts &, Books &, Log &);
  void CheckPrivilege(Accounts &);
  instruction_t GetString() const;
};
class ReportEmployeeInst : public Instruction {
public:
  static const int instructionPrivilege = 7;

  explicit ReportEmployeeInst(int);

  void Execute(Accounts &, Books &, Log &);
  void CheckParameter(Accounts &, Books &, Log &);
  void CheckPrivilege(Accounts &);
  instruction_t GetString() const;
};
class LogInst : public Instruction {
public:
  static const int instructionPrivilege = 7;

  explicit LogInst(int);

  void Execute(Accounts &, Books &, Log &);
  void CheckParameter(Accounts &, Books &, Log &);
  void CheckPrivilege(Accounts &);
  instruction_t GetString() const;
};

#endif //BOOKSTORE_2023_INSTRUCTION_H