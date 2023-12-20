#ifndef BOOKSTORE_2023_INSTRUCTION_H
#define BOOKSTORE_2023_INSTRUCTION_H

#include <string>

class Instruction {
public:
  int privilege_;

  virtual void Execute() = 0;
  virtual void CheckParameter() = 0;
  virtual std::string GetString() = 0;
};
class QuitInst : public Instruction {
public:
  QuitInst();

  void Execute();
  void CheckParameter();
  std::string GetString();

private:

};
class SuInst : public Instruction {
public:
  SuInst();

  void Execute();
  void CheckParameter();
  std::string GetString();

private:

};
class LogoutInst : public Instruction {
public:
  LogoutInst();

  void Execute();
  void CheckParameter();
  std::string GetString();

private:

};
class RegisterInst : public Instruction {
public:
  RegisterInst();

  void Execute();
  void CheckParameter();
  std::string GetString();

private:

};
class PasswdInst : public Instruction {
public:
  PasswdInst();

  void Execute();
  void CheckParameter();
  std::string GetString();

private:

};
class DeleteInst : public Instruction {
public:
  DeleteInst();

  void Execute();
  void CheckParameter();
  std::string GetString();

private:

};
class ShowInst : public Instruction {
public:
  ShowInst();

  void Execute();
  void CheckParameter();
  std::string GetString();

private:

};
class BuyInst : public Instruction {
public:
  BuyInst();

  void Execute();
  void CheckParameter();
  std::string GetString();

private:

};
class SelectInst : public Instruction {
public:
  SelectInst();

  void Execute();
  void CheckParameter();
  std::string GetString();

private:

};
class ModifyInst : public Instruction {
public:
  ModifyInst();

  void Execute();
  void CheckParameter();
  std::string GetString();

private:

};
class ImportInst : public Instruction {
public:
  ImportInst();

  void Execute();
  void CheckParameter();
  std::string GetString();

private:

};
class ShowFinanceInst : public Instruction {
public:
  ShowFinanceInst();

  void Execute();
  void CheckParameter();
  std::string GetString();

private:

};
class ReportFinanceInst : public Instruction {
public:
  ReportFinanceInst();

  void Execute();
  void CheckParameter();
  std::string GetString();

private:

};
class ReportEmployeeInst : public Instruction {
public:
  ReportEmployeeInst();

  void Execute();
  void CheckParameter();
  std::string GetString();

private:

};
class LogInst : public Instruction {
public:
  LogInst();

  void Execute();
  void CheckParameter();
  std::string GetString();

private:

};
class SuInstruction : public Instruction {
public:
  SuInstruction();

  void Execute();
  void CheckParameter();
  std::string GetString();

private:

};

#endif //BOOKSTORE_2023_INSTRUCTION_H
