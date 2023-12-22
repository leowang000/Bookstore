#ifndef BOOKSTORE_2023_USER_H
#define BOOKSTORE_2023_USER_H

#include <string>
#include <vector>

#include "Tools/String.h"
#include "Tools/File.h"
#include "Tools/UnrolledLinkedList.h"
#include "type_definition.h"

class Accounts {
public:
  struct User {
    user_id_t id_;
    password_t password_;
    username_t username_;
    privilege_t privilege_;

    User();
    explicit User(const user_id_t &, const password_t &, const username_t &, const privilege_t &);

    user_t GetString() const;
    bool operator<(const User &) const;
    bool operator==(const User &) const;
    bool operator>(const User &) const;
    bool operator<=(const User &) const;
    bool operator>=(const User &) const;
    bool operator!=(const User &) const;
  };

  explicit Accounts(char *, char *);

  bool HaveUser(const user_id_t &);
  bool IsUserEmpty() const;
  void LogOnUser(const user_id_t &);
  void LogOutUser();
  void AddUser(const User &);
  void DeleteUser(const user_id_t &);
  void ModifyPassword(const user_id_t &, const password_t &);
  int GetNowUserPrivilege() const;
  bool IsInstructionPrivilegeValid(int) const;
  User GetUser(const user_id_t &);
  void Select(int);
  int GetSelect() const;
  bool IsUserLoggedOn(const user_id_t &) const;

private:
  UnrolledLinkedList<user_id_t, User> accounts_map_;
  std::vector<std::pair<User, int>> users_;
};

#endif //BOOKSTORE_2023_USER_H