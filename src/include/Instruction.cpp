#include "Instruction.h"
#include "Tools/error.h"

Instruction::Instruction(int time) : time_(time) {}
bool Instruction::Execute(Accounts &accounts, Books &books, Log &log) {
  return false;
}
void Instruction::CheckParameter(Accounts &accounts, Books &books, Log &log) {}
void Instruction::CheckPrivilege(Accounts &accounts) {}
std::string Instruction::GetString() const {
  return time_.GetString() + " null";
}
QuitInst::QuitInst(int time, const std::string &op) : Instruction(time), is_quit(op == "quit") {}
bool QuitInst::Execute(Accounts &accounts, Books &books, Log &log) {
  return true;
}
void QuitInst::CheckParameter(Accounts &accounts, Books &books, Log &log) {}
void QuitInst::CheckPrivilege(Accounts &accounts) {
  if (!accounts.IsInstructionPrivilegeValid(instructionPrivilege)) {
    throw ErrorException("INSUFFICIENT PERMISSIONS");
  }
}
std::string QuitInst::GetString() const {
  return time_.GetString() + (is_quit ? "quit" : "exit");
}
SuInst::SuInst(int time, const std::string &user_id, const std::string &password)
    : Instruction(time), user_id_(user_id), password_(password) {}
bool SuInst::Execute(Accounts &accounts, Books &books, Log &log) {
  accounts.LogOnUser(user_id_);
  return false;
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
std::string SuInst::GetString() const {
  return time_.GetString() + " su " + user_id_.GetString() + " " + password_.GetString();
}
LogoutInst::LogoutInst(int time) : Instruction(time) {}
bool LogoutInst::Execute(Accounts &accounts, Books &books, Log &log) {
  accounts.LogOutUser();
  return false;
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
std::string LogoutInst::GetString() const {
  return time_.GetString() + " logout";
}
RegisterInst::RegisterInst(int time, const user_id_t &user_id, const password_t &password, const username_t &username)
    : Instruction(time), user_id_(user_id), password_(password), username_(username) {}
bool RegisterInst::Execute(Accounts &accounts, Books &books, Log &log) {
  accounts.AddUser(Accounts::User(user_id_, password_, username_, privilege_t("1")));
  return false;
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
std::string RegisterInst::GetString() const {
  return time_.GetString() + " register " + user_id_.GetString() + " " + password_.GetString() + " " +
      username_.GetString();
}
PasswdInst::PasswdInst(int time, const user_id_t &user_id, const password_t &current_password,
                       const password_t &new_password)
    : Instruction(time), user_id_(user_id), current_password_(current_password), new_password_(new_password) {}
bool PasswdInst::Execute(Accounts &accounts, Books &books, Log &log) {
  accounts.ModifyPassword(user_id_, new_password_);
  return false;
}
void PasswdInst::CheckParameter(Accounts &accounts, Books &books, Log &log) {
  if (!accounts.HaveUser(user_id_)) {
    throw ErrorException("INVALID USERID");
  }
  Accounts::User user(accounts.GetUser(user_id_));
  if (!current_password_.Empty() && current_password_ != user.password_) {
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
std::string PasswdInst::GetString() const {
  return time_.GetString() + " passwd " + user_id_.GetString() + " " +
      (current_password_ == "" ? "" : current_password_.GetString() + " ") + new_password_.GetString();
}
UserAddInst::UserAddInst(int time, const user_id_t &user_id, const password_t &password, const privilege_t &privilege,
                         const username_t &username)
    : Instruction(time), user_id_(user_id), password_(password), privilege_(privilege), username_(username) {}
bool UserAddInst::Execute(Accounts &accounts, Books &books, Log &log) {
  accounts.AddUser(Accounts::User(user_id_, password_, username_, privilege_));
  return false;
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
std::string UserAddInst::GetString() const {
  return time_.GetString() + " useradd " + user_id_.GetString() + " " + password_.GetString() + " " +
      privilege_.GetString() + " " + username_.GetString();
}
DeleteInst::DeleteInst(int time, const user_id_t &user_id) : Instruction(time), user_id_(user_id) {}
bool DeleteInst::Execute(Accounts &accounts, Books &books, Log &log) {
  accounts.DeleteUser(user_id_);
  return false;
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
std::string DeleteInst::GetString() const {
  return time_.GetString() + " delete " + user_id_.GetString();
}
ShowInst::ShowInst(int time, const Books::Book &show_index) : Instruction(time), search_index_(show_index) {}
bool ShowInst::Execute(Accounts &accounts, Books &books, Log &log) {
  std::vector<int> line_nums;
  if (search_index_.ISBN_.Empty() && search_index_.author_.Empty() && search_index_.book_name_.Empty() &&
  search_index_.keyword_.Empty()) {
    books.PrintBookFile(std::cout);
    return false;
  }
  if (!search_index_.ISBN_.Empty()) {
    line_nums = books.FindByISBN(search_index_.ISBN_);
  }
  else if (!search_index_.author_.Empty()) {
    line_nums = books.FindByAuthor(search_index_.author_);
  }
  else if (!search_index_.book_name_.Empty()) {
    line_nums = books.FindByBookName(search_index_.book_name_);
  }
  else if (!search_index_.keyword_.Empty()) {
    line_nums = books.FindByKeyword(search_index_.keyword_);
  }
  books.PrintBooks(std::cout, line_nums);
  return false;
}
void ShowInst::CheckParameter(Accounts &accounts, Books &books, Log &log) {}
void ShowInst::CheckPrivilege(Accounts &accounts) {
  if (!accounts.IsInstructionPrivilegeValid(instructionPrivilege)) {
    throw ErrorException("INSUFFICIENT PERMISSIONS");
  }
}
std::string ShowInst::GetString() const {
  std::string result = time_.GetString() + " show ";
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
  return result;
}
BuyInst::BuyInst(int time, const ISBN_t &ISBN, const quantity_t &quantity)
    : Instruction(time), ISBN_(ISBN), quantity_(quantity) {}
bool BuyInst::Execute(Accounts &accounts, Books &books, Log &log) {
  Books::Book book(books.GetBook(ISBN_));
  long double income = book.price_.ToDouble(2) * quantity_.ToLongLong();
  book.quantity_ = quantity_t(book.quantity_.ToLongLong() - quantity_.ToLongLong());
  books.Modify(book, books.FindByISBN(ISBN_).front());
  std::cout << std::fixed << std::setprecision(2) << income << "\n";
  FinanceInfo finance_info(log.GetLastFinanceInfo());
  finance_info.time_ = time_.ToInt();
  finance_info.income_ += income;
  log.AddFinanceInfo(finance_info);
  return false;
}
void BuyInst::CheckParameter(Accounts &accounts, Books &books, Log &log) {
  if (!books.HaveISBN(ISBN_)) {
    throw ErrorException("ISBN DOESN'T EXIST");
  }
  if (quantity_.ToLongLong() == 0 || quantity_.ToLongLong() > 2147483647ll ||
  quantity_.ToLongLong() > books.GetBook(ISBN_).quantity_.ToLongLong()) {
    throw ErrorException("INVALID QUANTITY");
  }
}
void BuyInst::CheckPrivilege(Accounts &accounts) {
  if (!accounts.IsInstructionPrivilegeValid(instructionPrivilege)) {
    throw ErrorException("INSUFFICIENT PERMISSIONS");
  }
}
std::string BuyInst::GetString() const {
  return time_.GetString() + " buy " + ISBN_.GetString() + " " + quantity_.GetString();
}
SelectInst::SelectInst(int time, const ISBN_t &ISBN) : Instruction(time), ISBN_(ISBN) {}
bool SelectInst::Execute(Accounts &accounts, Books &books, Log &log) {
  if (books.HaveISBN(ISBN_)) {
    accounts.Select(books.FindByISBN(ISBN_).front());
    return false;
  }
  Books::Book book(ISBN_.ToString());
  accounts.Select(books.AddBook(book));
  return false;
}
void SelectInst::CheckParameter(Accounts &accounts, Books &books, Log &log) {}
void SelectInst::CheckPrivilege(Accounts &accounts) {
  if (!accounts.IsInstructionPrivilegeValid(instructionPrivilege)) {
    throw ErrorException("INSUFFICIENT PERMISSIONS");
  }
}
std::string SelectInst::GetString() const {
  return time_.GetString() + " select " + ISBN_.GetString();
}
ModifyInst::ModifyInst(int time, const Books::Book &modification) : Instruction(time), modification_(modification) {}
bool ModifyInst::Execute(Accounts &accounts, Books &books, Log &log) {
  books.Modify(modification_, accounts.GetSelect());
  return false;
}
void ModifyInst::CheckParameter(Accounts &accounts, Books &books, Log &log) {
  if (accounts.GetSelect() == -1) {
    throw ErrorException("NO BOOK SELECTED");
  }
  if (books.HaveISBN(modification_.ISBN_)) {
    throw ErrorException("NEW ISBN IS SAME AS PREVIOUS ISBN");
  }
}
void ModifyInst::CheckPrivilege(Accounts &accounts) {
  if (!accounts.IsInstructionPrivilegeValid(instructionPrivilege)) {
    throw ErrorException("INSUFFICIENT PERMISSIONS");
  }
}
std::string ModifyInst::GetString() const {
  std::string result = time_.GetString() + " modify";
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
  return result;
}
ImportInst::ImportInst(int time, const quantity_t &quantity, const price_t &cost)
    : Instruction(time), quantity_(quantity), cost_(cost) {}
bool ImportInst::Execute(Accounts &accounts, Books &books, Log &log) {
  Books::Book book(books.GetBook(accounts.GetSelect()));
  book.quantity_ = quantity_t(book.quantity_.ToLongLong() + quantity_.ToLongLong());
  books.Modify(book, books.FindByISBN(book.ISBN_).front());
  FinanceInfo finance_info(log.GetLastFinanceInfo());
  finance_info.time_ = time_.ToInt();
  finance_info.outcome_ += cost_.ToDouble(2);
  log.AddFinanceInfo(finance_info);
  return false;
}
void ImportInst::CheckParameter(Accounts &accounts, Books &books, Log &log) {
  if (accounts.GetSelect() == -1) {
    throw ErrorException("NO BOOK SELECTED");
  }
  if (quantity_.ToLongLong() == 0 || quantity_.ToLongLong() > 2147483647ll) {
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
std::string ImportInst::GetString() const {
  return time_.GetString() + " import " + quantity_.GetString() + " " + cost_.GetString(2);
}
ShowFinanceInst::ShowFinanceInst(int time, const count_t &count) : Instruction(time), count_(count) {}
bool ShowFinanceInst::Execute(Accounts &accounts, Books &books, Log &log) {
  log.PrintFinanceInfo(std::cout, (count_.Empty() ? -1 : count_.ToInt()));
  return false;
}
void ShowFinanceInst::CheckParameter(Accounts &accounts, Books &books, Log &log) {
  if (count_.ToLongLong() > 2147483647ll || count_.ToInt() > log.GetFinanceFileLength()) {
    throw ErrorException("INVALID COUNT");
  }
}
void ShowFinanceInst::CheckPrivilege(Accounts &accounts) {
  if (!accounts.IsInstructionPrivilegeValid(instructionPrivilege)) {
    throw ErrorException("INSUFFICIENT PERMISSIONS");
  }
}
std::string ShowFinanceInst::GetString() const {
  return time_.GetString() + "show finance" + (count_.Empty() ? "" : " " + count_.GetString());
}
ReportFinanceInst::ReportFinanceInst(int time) : Instruction(time) {}
bool ReportFinanceInst::Execute(Accounts &accounts, Books &books, Log &log) {
  log.PrintFinance(std::cout);
  return false;
}
void ReportFinanceInst::CheckParameter(Accounts &accounts, Books &books, Log &log) {}
void ReportFinanceInst::CheckPrivilege(Accounts &accounts) {
  if (!accounts.IsInstructionPrivilegeValid(instructionPrivilege)) {
    throw ErrorException("INSUFFICIENT PERMISSIONS");
  }
}
std::string ReportFinanceInst::GetString() const {
  return time_.GetString() + " report finance";
}
ReportEmployeeInst::ReportEmployeeInst(int time) : Instruction(time) {}
bool ReportEmployeeInst::Execute(Accounts &accounts, Books &books, Log &log) {
  log.PrintEmployee(std::cout);
  return false;
}
void ReportEmployeeInst::CheckParameter(Accounts &accounts, Books &books, Log &log) {}
void ReportEmployeeInst::CheckPrivilege(Accounts &accounts) {
  if (!accounts.IsInstructionPrivilegeValid(instructionPrivilege)) {
    throw ErrorException("INSUFFICIENT PERMISSIONS");
  }
}
std::string ReportEmployeeInst::GetString() const {
  return time_.GetString() + " report employee";
}
LogInst::LogInst(int time) : Instruction(time) {}
bool LogInst::Execute(Accounts &accounts, Books &books, Log &log) {
  log.PrintLogFile(std::cout);
  return false;
}
void LogInst::CheckParameter(Accounts &accounts, Books &books, Log &log) {}
void LogInst::CheckPrivilege(Accounts &accounts) {
  if (!accounts.IsInstructionPrivilegeValid(instructionPrivilege)) {
    throw ErrorException("INSUFFICIENT PERMISSIONS");
  }
}
std::string LogInst::GetString() const {
  return time_.GetString() + " log";
}