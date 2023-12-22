#include "Instruction.h"
#include "Tools/error.h"

Instruction::Instruction() : time_(-1) {}
Instruction::Instruction(int time) : time_(time) {}
QuitInst::QuitInst(int time, const std::string &op) : Instruction(time), is_quit(op == "quit") {}
void QuitInst::Execute(Accounts &accounts, Books &books, Log &log) {
  exit(0);
}
void QuitInst::CheckParameter(Accounts &accounts, Books &books, Log &log) {}
void QuitInst::CheckPrivilege(Accounts &accounts) {
  if (!accounts.IsInstructionPrivilegeValid(instructionPrivilege)) {
    throw ErrorException("INSUFFICIENT PERMISSIONS");
  }
}
instruction_t QuitInst::GetString() const {
  return { is_quit ? "quit" : "exit" };
}
SuInst::SuInst(int time, const std::string &user_id, const std::string &password)
    : Instruction(time), user_id_(user_id), password_(password) {}
void SuInst::Execute(Accounts &accounts, Books &books, Log &log) {
  accounts.LogOnUser(user_id_);
}
void SuInst::CheckParameter(Accounts &accounts, Books &books, Log &log) {
  if (!accounts.HaveUser(user_id_)) {
    throw ErrorException("USERID DOESN'T EXIST");
  }
  Accounts::User user(accounts.GetUser(user_id_));
  if (!password_.Empty() && password_ != user.password_) {
    throw ErrorException("WRONG PASSWORD");
  }
  if (password_.Empty() && accounts.GetNowUserPrivilege() <= user.privilege_.ToInt()) {
    throw ErrorException("PASSWORD MISSING");
  }
}
void SuInst::CheckPrivilege(Accounts &accounts) {
  if (!accounts.IsInstructionPrivilegeValid(instructionPrivilege)) {
    throw ErrorException("INSUFFICIENT PERMISSIONS");
  }
}
instruction_t SuInst::GetString() const {
  return { "su " + user_id_.GetString() + " " + password_.GetString() };
}
LogoutInst::LogoutInst(int time) : Instruction(time) {}
void LogoutInst::Execute(Accounts &accounts, Books &books, Log &log) {
  accounts.LogOutUser();
}
void LogoutInst::CheckParameter(Accounts &accounts, Books &books, Log &log) {
  if (accounts.IsUserEmpty()) {
    throw ErrorException("NO USER TO LOGOUT");
  }
}
void LogoutInst::CheckPrivilege(Accounts &accounts) {
  if (!accounts.IsInstructionPrivilegeValid(instructionPrivilege)) {
    throw ErrorException("INSUFFICIENT PERMISSIONS");
  }
}
instruction_t LogoutInst::GetString() const {
  return { "logout" };
}
RegisterInst::RegisterInst(int time, const user_id_t &user_id, const password_t &password, const username_t &username)
    : Instruction(time), user_id_(user_id), password_(password), username_(username) {}
void RegisterInst::Execute(Accounts &accounts, Books &books, Log &log) {
  accounts.AddUser(Accounts::User(user_id_, password_, username_, privilege_t("1")));
}
void RegisterInst::CheckParameter(Accounts &accounts, Books &books, Log &log) {
  if (accounts.HaveUser(user_id_)) {
    throw ErrorException("USERID EXISTS");
  }
}
void RegisterInst::CheckPrivilege(Accounts &accounts) {
  if (!accounts.IsInstructionPrivilegeValid(instructionPrivilege)) {
    throw ErrorException("INSUFFICIENT PERMISSIONS");
  }
}
instruction_t RegisterInst::GetString() const {
  return { "register " + user_id_.GetString() + " " + password_.GetString() + " " + username_.GetString()};
}
PasswdInst::PasswdInst(int time, const user_id_t &user_id, const password_t &current_password,
                       const password_t &new_password)
    : Instruction(time), user_id_(user_id), current_password_(current_password), new_password_(new_password) {}
void PasswdInst::Execute(Accounts &accounts, Books &books, Log &log) {
  accounts.ModifyPassword(user_id_, new_password_);
}
void PasswdInst::CheckParameter(Accounts &accounts, Books &books, Log &log) {
  if (!accounts.HaveUser(user_id_)) {
    throw ErrorException("INVALID USERID");
  }
  Accounts::User user(accounts.GetUser(user_id_));
  if (!current_password_.Empty() && current_password_ != user.id_) {
    throw ErrorException("WRONG PASSWORD");
  }
  if (current_password_.Empty() && accounts.GetNowUserPrivilege() != 7) {
    throw ErrorException("PASSWORD MISSING");
  }
}
void PasswdInst::CheckPrivilege(Accounts &accounts) {
  if (!accounts.IsInstructionPrivilegeValid(instructionPrivilege)) {
    throw ErrorException("INSUFFICIENT PERMISSIONS");
  }
}
instruction_t PasswdInst::GetString() const {
  return { "passwd " + user_id_.GetString() + " " +
      (current_password_ == "" ? "" : current_password_.GetString() + " ") + new_password_.GetString() };
}
UserAddInst::UserAddInst(int time, const user_id_t &user_id, const password_t &password, const privilege_t &privilege,
                         const username_t &username)
    : Instruction(time), user_id_(user_id), password_(password), privilege_(privilege), username_(username) {}
void UserAddInst::Execute(Accounts &accounts, Books &books, Log &log) {
  accounts.AddUser(Accounts::User(user_id_, password_, username_, privilege_));
}
void UserAddInst::CheckParameter(Accounts &accounts, Books &books, Log &log) {
  int privilege = privilege_.ToInt();
  if (privilege != 1 && privilege != 3 && privilege != 7) {
    throw ErrorException("INVALID PRIVILEGE");
  }
  if (privilege >= accounts.GetNowUserPrivilege()) {
    throw ErrorException("INSUFFICIENT PERMISSIONS");
  }
  if (accounts.HaveUser(user_id_)) {
    throw ErrorException("USERID EXISTS");
  }
}
void UserAddInst::CheckPrivilege(Accounts &accounts) {
  if (!accounts.IsInstructionPrivilegeValid(instructionPrivilege)) {
    throw ErrorException("INSUFFICIENT PERMISSIONS");
  }
}
instruction_t UserAddInst::GetString() const {
  return { "useradd " + user_id_.GetString() + " " + password_.GetString() + " " + privilege_.GetString() + " " +
      username_.GetString() };
}
DeleteInst::DeleteInst(int time, const user_id_t &user_id) : Instruction(time), user_id_(user_id) {}
void DeleteInst::Execute(Accounts &accounts, Books &books, Log &log) {
  accounts.DeleteUser(user_id_);
}
void DeleteInst::CheckParameter(Accounts &accounts, Books &books, Log &log) {
  if (!accounts.HaveUser(user_id_)) {
    throw ErrorException("USERID DOESN'T EXIST");
  }
  if (accounts.IsUserLoggedOn(user_id_)) {
    throw ErrorException("USER ALREADY LOGGED ON");
  }
}
void DeleteInst::CheckPrivilege(Accounts &accounts) {
  if (!accounts.IsInstructionPrivilegeValid(instructionPrivilege)) {
    throw ErrorException("INSUFFICIENT PERMISSIONS");
  }
}
instruction_t DeleteInst::GetString() const {
  return { "delete " + user_id_.GetString() };
}
ShowInst::ShowInst(int time, const Books::Book &show_index) : Instruction(time), search_index_(show_index) {}
void ShowInst::Execute(Accounts &accounts, Books &books, Log &log) {
  std::vector<int> line_nums;
  if (search_index_.ISBN_.Empty() && search_index_.author_.Empty() && search_index_.book_name_.Empty() &&
  search_index_.keyword_.Empty()) {
    books.PrintBookFile(std::cout);
    return;
  }
  if (!search_index_.ISBN_.Empty()) {
    line_nums = books.FindByISBN(search_index_.ISBN_);
  }
  if (!search_index_.author_.Empty()) {
    line_nums = books.FindByAuthor(search_index_.author_);
  }
  if (!search_index_.book_name_.Empty()) {
    line_nums = books.FindByBookName(search_index_.book_name_);
  }
  if (!search_index_.keyword_.Empty()) {
    line_nums = books.FindByKeyword(search_index_.keyword_);
  }
  books.PrintBooks(std::cout, line_nums);
}
void ShowInst::CheckParameter(Accounts &accounts, Books &books, Log &log) {}
void ShowInst::CheckPrivilege(Accounts &accounts) {
  if (!accounts.IsInstructionPrivilegeValid(instructionPrivilege)) {
    throw ErrorException("INSUFFICIENT PERMISSIONS");
  }
}
instruction_t ShowInst::GetString() const {
  std::string result = "show ";
  if (!search_index_.ISBN_.Empty()) {
    result += "-ISBN=" + search_index_.ISBN_.GetString();
  }
  if (!search_index_.author_.Empty()) {
    result += "-author=" + search_index_.author_.GetString();
  }
  if (!search_index_.book_name_.Empty()) {
    result += "-name=" + search_index_.book_name_.GetString();
  }
  if (!search_index_.keyword_.Empty()) {
    result += "-keyword=" + search_index_.keyword_.GetString();
  }
  return { result };
}
BuyInst::BuyInst(int time, const ISBN_t &ISBN, const quantity_t &quantity)
    : Instruction(time), ISBN_(ISBN), quantity_(quantity) {}
void BuyInst::Execute(Accounts &accounts, Books &books, Log &log) {
  Books::Book book(books.GetBook(ISBN_));
  long double income = book.price_.ToDouble(2) * quantity_.ToInt();
  book.quantity_ = quantity_t(book.quantity_.ToInt() - quantity_.ToInt());
  books.Modify(book, books.FindByISBN(ISBN_).front());
  std::cout << std::fixed << std::setprecision(2) << income << "\n";
  FinanceInfo finance_info(log.GetLastFinanceInfo());
  finance_info.time_ = time_;
  finance_info.income_ += income;
  log.AddFinanceInfo(finance_info);
}
void BuyInst::CheckParameter(Accounts &accounts, Books &books, Log &log) {
  if (!books.HaveISBN(ISBN_)) {
    throw ErrorException("ISBN DOESN'T EXIST");
  }
  if (quantity_.ToInt() == 0 || quantity_.ToInt() > books.GetBook(ISBN_).quantity_.ToInt()) {
    throw ErrorException("INVALID QUANTITY");
  }
}
void BuyInst::CheckPrivilege(Accounts &accounts) {
  if (!accounts.IsInstructionPrivilegeValid(instructionPrivilege)) {
    throw ErrorException("INSUFFICIENT PERMISSIONS");
  }
}
instruction_t BuyInst::GetString() const {
  return { "buy " + ISBN_.GetString() + " " + quantity_.GetString() };
}
SelectInst::SelectInst(int time, const ISBN_t &ISBN) : Instruction(time), ISBN_(ISBN) {}
void SelectInst::Execute(Accounts &accounts, Books &books, Log &log) {
  if (books.HaveISBN(ISBN_)) {
    accounts.Select(books.FindByISBN(ISBN_).front());
    return;
  }
  Books::Book book(ISBN_.ToString());
  accounts.Select(books.AddBook(book));
}
void SelectInst::CheckParameter(Accounts &accounts, Books &books, Log &log) {}
void SelectInst::CheckPrivilege(Accounts &accounts) {
  if (!accounts.IsInstructionPrivilegeValid(instructionPrivilege)) {
    throw ErrorException("INSUFFICIENT PERMISSIONS");
  }
}
instruction_t SelectInst::GetString() const {
  return { "select " + ISBN_.GetString() };
}
ModifyInst::ModifyInst(int time, const Books::Book &modification) : Instruction(time), modification_(modification) {}
void ModifyInst::Execute(Accounts &accounts, Books &books, Log &log) {
  books.Modify(modification_, accounts.GetSelect());
}
void ModifyInst::CheckParameter(Accounts &accounts, Books &books, Log &log) {
  if (accounts.GetSelect() == -1) {
    throw ErrorException("NO BOOK SELECTED");
  }
  if (modification_.ISBN_ == books.GetBook(accounts.GetSelect()).ISBN_) {
    throw ErrorException("NEW ISBN IS SAME AS PREVIOUS ISBN");
  }
}
void ModifyInst::CheckPrivilege(Accounts &accounts) {
  if (!accounts.IsInstructionPrivilegeValid(instructionPrivilege)) {
    throw ErrorException("INSUFFICIENT PERMISSIONS");
  }
}
instruction_t ModifyInst::GetString() const {
  std::string result = "modify";
  if (!modification_.ISBN_.Empty()) {
    result += " -ISBN=" + modification_.ISBN_.GetString();
  }
  if (!modification_.book_name_.Empty()) {
    result += " -name=" + modification_.book_name_.GetString();
  }
  if (!modification_.author_.Empty()) {
    result += " -author=" + modification_.author_.GetString();
  }
  if (!modification_.keyword_.Empty()) {
    result += " -keyword=" + modification_.keyword_.GetString();
  }
  return { result };
}
ImportInst::ImportInst(int time, const quantity_t &quantity, const price_t &cost)
    : Instruction(time), quantity_(quantity), cost_(cost) {}
void ImportInst::Execute(Accounts &accounts, Books &books, Log &log) {
  Books::Book book(books.GetBook(accounts.GetSelect()));
  book.quantity_ = quantity_t(book.quantity_.ToInt() + quantity_.ToInt());
  books.Modify(book, books.FindByISBN(book.ISBN_).front());
  FinanceInfo finance_info(log.GetLastFinanceInfo());
  finance_info.time_ = time_;
  finance_info.outcome_ += cost_.ToDouble(2);
  log.AddFinanceInfo(finance_info);
}
void ImportInst::CheckParameter(Accounts &accounts, Books &books, Log &log) {
  if (accounts.GetSelect() == -1) {
    throw ErrorException("NO BOOK SELECTED");
  }
  if (quantity_.ToInt() == 0) {
    throw ErrorException("INVALID QUANTITY");
  }
  if (cost_.ToDouble(2) == 0) {
    throw ErrorException("INVALID TOTAL COST");
  }
}
void ImportInst::CheckPrivilege(Accounts &accounts) {
  if (!accounts.IsInstructionPrivilegeValid(instructionPrivilege)) {
    throw ErrorException("INSUFFICIENT PERMISSIONS");
  }
}
instruction_t ImportInst::GetString() const {
  return { "import " + quantity_.GetString() + " " + cost_.GetString(2) };
}
ShowFinanceInst::ShowFinanceInst(int time, const count_t &count) : Instruction(time), count_(count) {}
void ShowFinanceInst::Execute(Accounts &accounts, Books &books, Log &log) {
  log.PrintFinanceInfo(std::cout, (count_.Empty() ? -1 : count_.ToInt()));
}
void ShowFinanceInst::CheckParameter(Accounts &accounts, Books &books, Log &log) {
  if (count_.ToInt() > log.GetFinanceFileLength()) {
    throw ErrorException("INVALID COUNT");
  }
}
void ShowFinanceInst::CheckPrivilege(Accounts &accounts) {
  if (!accounts.IsInstructionPrivilegeValid(instructionPrivilege)) {
    throw ErrorException("INSUFFICIENT PERMISSIONS");
  }
}
instruction_t ShowFinanceInst::GetString() const {
  return { "show finance" + (count_.Empty() ? "" : " " + count_.GetString()) };
}
ReportFinanceInst::ReportFinanceInst(int time) : Instruction(time) {}
void ReportFinanceInst::Execute(Accounts &accounts, Books &books, Log &log) {
  log.PrintFinance(std::cout);
}
void ReportFinanceInst::CheckParameter(Accounts &accounts, Books &books, Log &log) {}
void ReportFinanceInst::CheckPrivilege(Accounts &accounts) {
  if (!accounts.IsInstructionPrivilegeValid(instructionPrivilege)) {
    throw ErrorException("INSUFFICIENT PERMISSIONS");
  }
}
instruction_t ReportFinanceInst::GetString() const {
  return { "report finance" };
}
ReportEmployeeInst::ReportEmployeeInst(int time) : Instruction(time) {}
void ReportEmployeeInst::Execute(Accounts &accounts, Books &books, Log &log) {
  log.PrintEmployee(std::cout);
}
void ReportEmployeeInst::CheckParameter(Accounts &accounts, Books &books, Log &log) {}
void ReportEmployeeInst::CheckPrivilege(Accounts &accounts) {
  if (!accounts.IsInstructionPrivilegeValid(instructionPrivilege)) {
    throw ErrorException("INSUFFICIENT PERMISSIONS");
  }
}
instruction_t ReportEmployeeInst::GetString() const {
  return { "report employee" };
}
LogInst::LogInst(int time) : Instruction(time) {}
void LogInst::Execute(Accounts &accounts, Books &books, Log &log) {
  log.PrintLogFile(std::cout);
}
void LogInst::CheckParameter(Accounts &accounts, Books &books, Log &log) {}
void LogInst::CheckPrivilege(Accounts &accounts) {
  if (!accounts.IsInstructionPrivilegeValid(instructionPrivilege)) {
    throw ErrorException("INSUFFICIENT PERMISSIONS");
  }
}
instruction_t LogInst::GetString() const {
  return { "log" };
}