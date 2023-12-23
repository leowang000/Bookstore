#include "Accounts.h"

Accounts::User::User() : id_(), password_(), username_(), privilege_("0") {}
Accounts::User::User(const user_id_t &id, const password_t &password, const username_t &username,
                     const privilege_t &privilege)
    : id_(id), password_(password), username_(username), privilege_(privilege) {}
std::string Accounts::User::GetString() const {
  return id_.GetString() + " {" + privilege_.GetString() + "} " + username_.GetString();
}
bool Accounts::User::operator<(const User &rhs) const {
  return id_ < rhs.id_;
}
bool Accounts::User::operator==(const User &rhs) const {
  return id_ == rhs.id_;
}
bool Accounts::User::operator>(const User &rhs) const {
  return id_ > rhs.id_;
}
bool Accounts::User::operator<=(const User &rhs) const {
  return id_ <= rhs.id_;
}bool Accounts::User::operator>=(const User &rhs) const {
  return id_ >= rhs.id_;
}
bool Accounts::User::operator!=(const User &rhs) const {
  return id_ != rhs.id_;
}

Accounts::Accounts(char *account_data_file_name, char *account_node_file_name)
    : accounts_map_(account_data_file_name, account_node_file_name), users_() {}
void Accounts::Init() {
  if (accounts_map_.Empty()) {
    User root_user("root", "sjtu", "bubble", "7");
    accounts_map_.Insert("root", root_user);
  }
}
bool Accounts::HaveUser(const user_id_t &user_id) {
  return accounts_map_.Have(user_id);
}
bool Accounts::IsUserEmpty() const {
  return users_.empty();
}
void Accounts::LogOnUser(const user_id_t &user_id) {
  users_.emplace_back(accounts_map_.Find(user_id).front(), -1);
}
void Accounts::LogOutUser() {
  users_.pop_back();
}
void Accounts::AddUser(const User &user) {
  accounts_map_.Insert(user.id_, user);
}
void Accounts::DeleteUser(const user_id_t &user_id) {
  accounts_map_.Delete(user_id, accounts_map_.Find(user_id).front());
}
void Accounts::ModifyPassword(const user_id_t &user_id, const password_t &new_password) {
  User user = accounts_map_.Find(user_id).front();
  accounts_map_.Delete(user_id, user);
  user.password_ = new_password;
  accounts_map_.Insert(user_id, user);
  for (auto &us : users_) {
    if (us.first.id_ == user_id) {
      us.first.password_ = new_password;
    }
  }
}
int Accounts::GetNowUserPrivilege() const {
  return users_.empty() ? 0 : users_.back().first.privilege_.ToInt();
}
std::string Accounts::GetNowUserString() const {
  if (!users_.empty()) {
    return users_.back().first.GetString();
  }
  return  user_id_t().GetString() + " {0} " + username_t().GetString();
}
bool Accounts::IsInstructionPrivilegeValid(int privilege) const {
  return GetNowUserPrivilege() >= privilege;
}
Accounts::User Accounts::GetUser(const user_id_t &user_id) {
  return accounts_map_.Find(user_id).front();
}
void Accounts::Select(int line_num) {
  users_.back().second = line_num;
}
int Accounts::GetSelect() const {
  return users_.back().second;
}
bool Accounts::IsUserLoggedOn(const user_id_t &user_id) const {
  for (auto &us : users_) {
    if (us.first.id_ == user_id) {
      return true;
    }
  }
  return false;
}