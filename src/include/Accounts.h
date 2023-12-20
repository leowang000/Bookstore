#ifndef BOOKSTORE_2023_USER_H
#define BOOKSTORE_2023_USER_H

#include <string>

#include "Tools/String.h"
#include "Tools/File.h"
#include "Tools/UnrolledLinkedList.h"

struct User {
  String<30> id_, password_, username_;
  int privilege, selected_;

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
class Accounts {
public:
  User now_user_;

  Accounts(char *, char *);

private:
  UnrolledLinkedList<String<30>, User> accounts_;

};

#endif //BOOKSTORE_2023_USER_H
