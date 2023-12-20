#ifndef BOOKSTORE_2023_BOOKS_H
#define BOOKSTORE_2023_BOOKS_H

#include <string>
#include <vector>

#include "Tools/String.h"
#include "Tools/UnrolledLinkedList.h"
#include "Tools/File.h"

class Books {
public:
  using ISBN_t = String<20>;
  using book_name_t = String<60>;
  using author_t = String<60>;
  using keyword_t = String<60>;
  using quantity_t = String<10>;
  using price_t = String<13>;
  struct Book {
    Book(const std::string &ISBN = "", const std::string &book_name = "", const std::string &author = "",
         const std::string &keyword = "", const std::string &quantity = "0", const std::string &price = "0");

    ISBN_t ISBN_;
    book_name_t book_name_;
    author_t author_;
    keyword_t keyword_;
    quantity_t quantity_;
    price_t price_;
    std::vector<std::string> keywords_;

    std::string GetString();
  };
  struct Modification {
    ISBN_t ISBN_;
    book_name_t book_name_;
    author_t author_;
    keyword_t keyword_;
    quantity_t quantity_;
    price_t price_;

    Modification(const std::string &ISBN = "", const std::string &book_name = "", const std::string &author = "",
                 const std::string &key_word = "", const std::string &quantity = "", const std::string &price = "");
  };

  Books(char *, char *, char *, char *, char *, char *, char *, char *, char *);

  std::vector<int> FindByISBN(const ISBN_t &);
  std::vector<int> FindByBookName(const book_name_t &);
  std::vector<int> FindByAuthor(const author_t &);
  std::vector<int> FindByKeyword(const keyword_t &);
  bool HaveISBN(const ISBN_t &);
  bool HaveBookName(const book_name_t &);
  bool HaveAuthor(const author_t &);
  bool HaveKeyword(const keyword_t &);
  void Modify(const Modification &, int);

private:
  int book_cnt_;
  File book_file_;
  UnrolledLinkedList<ISBN_t, int> ISBN_map_;
  UnrolledLinkedList<book_name_t, int> book_name_map_;
  UnrolledLinkedList<author_t, int> author_map_;
  UnrolledLinkedList<keyword_t, int> keyword_map_;

  void ReadBook(Book &, int);
  int WriteBook(Book &, int line_num = -1);
};

#endif //BOOKSTORE_2023_BOOKS_H