#ifndef BOOKSTORE_2023_USER_H
#define BOOKSTORE_2023_USER_H

#include <string>
#include <vector>

#include "Tools/String.h"
#include "Tools/File.h"
#include "Tools/UnrolledLinkedList.h"

class Accounts {
public:
  using user_id_t = String<30>;
  using password_t = String<30>;
  using username_t = String<30>;
  using privilege_t = String<1>;
  struct User {
    user_id_t id_;
    password_t password_;
    username_t username_;
    privilege_t privilege_;
    int selected_;

    User();
    User(const std::string &, const std::string &, const std::string &, const std::string &);

    std::string GetString();
    bool operator<(const User &) const;
    bool operator==(const User &) const;
    bool operator>(const User &) const;
    bool operator<=(const User &) const;
    bool operator>=(const User &) const;
    bool operator!=(const User &) const;
  };

  Accounts(char *, char *);

  bool HaveUser(const user_id_t &);
  bool IsUserEmpty() const;
  void LogOnUser(const user_id_t &);
  void LogOutUser();
  void AddUser(const User &);
  void DeleteUser(const user_id_t &);
  void ModifyPassword(const user_id_t &, const password_t &);

  User *now_user_;

private:
  UnrolledLinkedList<user_id_t, User> accounts_map_;
  std::vector<User> users_;
};

#endif //BOOKSTORE_2023_USER_H