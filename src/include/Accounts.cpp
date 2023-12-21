#include "Accounts.h"

Accounts::User::User() : id_(), password_(), username_(), privilege_("0"), selected_(-1) {}
Accounts::User::User(const std::string &id, const std::string &password, const std::string &username,
                     const std::string &privilege)
    : id_(id), password_(password), username_(username), privilege_(privilege), selected_(-1) {}
std::string Accounts::User::GetString() {
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
    : accounts_map_(account_data_file_name, account_node_file_name), users_(), now_user_(nullptr) {}
bool Accounts::HaveUser(const Accounts::user_id_t &user_id) {
  return accounts_map_.Have(user_id);
}
bool Accounts::IsUserEmpty() const {
  return users_.empty();
}
void Accounts::LogOnUser(const Accounts::user_id_t &user_id) {
  users_.emplace_back(accounts_map_.Find(user_id).front());
  now_user_ = &users_.back();
}
void Accounts::LogOutUser() {
  users_.pop_back();
  now_user_ = (users_.empty() ? nullptr : &users_.back());
}
void Accounts::AddUser(const User &user) {
  accounts_map_.Insert(user.id_, user);
}
void Accounts::DeleteUser(const Accounts::user_id_t &user_id) {
  accounts_map_.Delete(user_id, accounts_map_.Find(user_id).front());
}
void Accounts::ModifyPassword(const Accounts::user_id_t &user_id, const Accounts::password_t &new_password) {
  User user = accounts_map_.Find(user_id).front();
  accounts_map_.Delete(user_id, user);
  user.password_ = new_password;
  accounts_map_.Insert(user_id, user);
  for (auto &us : users_) {
    if (us.id_ == user_id) {
      us.password_ = new_password;
    }
  }
}