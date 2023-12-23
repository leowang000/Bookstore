#ifndef BOOKSTORE_2023_BOOKS_H
#define BOOKSTORE_2023_BOOKS_H

#include <string>
#include <vector>

#include "Tools/String.h"
#include "Tools/UnrolledLinkedList.h"
#include "Tools/File.h"
#include "type_definition.h"

class Books {
public:
  struct Book {
    explicit Book(const std::string &ISBN = "", const std::string &book_name = "", const std::string &author = "",
         const std::string &keyword = "", const std::string &quantity = "0", const std::string &price = "0");

    ISBN_t ISBN_;
    book_name_t book_name_;
    author_t author_;
    keyword_t keyword_;
    quantity_t quantity_;
    price_t price_;

    std::string GetString();
  };

  explicit Books(char *, char *, char *, char *, char *, char *, char *, char *, char *);

  std::vector<int> FindByISBN(const ISBN_t &);
  std::vector<int> FindByBookName(const book_name_t &);
  std::vector<int> FindByAuthor(const author_t &);
  std::vector<int> FindByKeyword(const keyword_t &);
  bool HaveISBN(const ISBN_t &);
  void Modify(Book &, int);
  int AddBook(Book &);
  void PrintBookFile(std::ostream &);
  void PrintBooks(std::ostream &, const std::vector<int> &);
  Book GetBook(const ISBN_t &);
  Book GetBook(int);

private:
  struct Line {
    int line_num_;
    ISBN_t ISBN_;

    Line();
    Line(int, const ISBN_t &);

    bool operator<(const Line &) const;
    bool operator==(const Line &) const;
    bool operator>(const Line &) const;
    bool operator<=(const Line &) const;
    bool operator>=(const Line &) const;
    bool operator!=(const Line &) const;
  };

  void ReadBook(Book &, int);
  int WriteBook(Book &, int line_num = -1);

  int book_cnt_;
  File book_file_;
  UnrolledLinkedList<ISBN_t, int> ISBN_map_;
  UnrolledLinkedList<book_name_t, Line> book_name_map_;
  UnrolledLinkedList<author_t, Line> author_map_;
  UnrolledLinkedList<keyword_t, Line> keyword_map_;
};

#endif //BOOKSTORE_2023_BOOKS_H