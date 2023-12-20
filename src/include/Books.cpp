#include "Books.h"
#include "Tools/read_string.h"

Books::Book::Book(const std::string &ISBN, const std::string &book_name, const std::string &author,
                  const std::string &keyword, const std::string &quantity, const std::string &price)
    : ISBN_(ISBN), book_name_(book_name), author_(author), keyword_(keyword), quantity_(quantity), price_(price),
    keywords_(ParseKeywords(keyword)) {}
std::string Books::Book::GetString() {
  return ISBN_.GetString() + "\t" + book_name_.GetString() + "\t" + author_.GetString() + "\t" + keyword_.GetString() +
  "\t" +  price_.GetString(2) + "\t" + quantity_.GetString(0) + "\n";
}
Books::Modification::Modification(const std::string &ISBN, const std::string &book_name, const std::string &author,
                                  const std::string &keyword, const std::string &quantity, const std::string &price)
    : ISBN_(ISBN), book_name_(book_name), author_(author), keyword_(keyword), quantity_(quantity), price_(price) {}
Books::Books(char *book_file_name, char *ISBN_data_file_name, char *ISBN_node_file_name,
             char *book_name_data_file_name, char *book_name_node_file_name, char *author_data_file_name,
             char *author_node_file_name, char *keyword_data_file_name, char *keyword_node_file_name)
    : book_cnt_(0), book_file_(book_file_name), ISBN_map_(ISBN_data_file_name, ISBN_node_file_name),
    book_name_map_(book_name_data_file_name, book_name_node_file_name),
    author_map_(author_data_file_name, author_node_file_name),
    keyword_map_(keyword_data_file_name, keyword_node_file_name) {}
std::vector<int> Books::FindByISBN(const Books::ISBN_t &ISBN) {
  return ISBN_map_.Find(ISBN);
}
std::vector<int> Books::FindByBookName(const Books::book_name_t &book_name) {
  return book_name_map_.Find(book_name);
}
std::vector<int> Books::FindByAuthor(const Books::author_t &author) {
  return author_map_.Find(author);
}
std::vector<int> Books::FindByKeyword(const Books::keyword_t &keyword) {
  return keyword_map_.Find(keyword);
}
bool Books::HaveISBN(const Books::ISBN_t &ISBN) {
  return ISBN_map_.Have(ISBN);
}
bool Books::HaveBookName(const Books::book_name_t &book_name) {
  return book_name_map_.Have(book_name);
}
bool Books::HaveAuthor(const Books::author_t &author) {
  return author_map_.Have(author);
}
bool Books::HaveKeyword(const Books::keyword_t &keyword) {
  return keyword_map_.Have(keyword);
}
void Books::Modify(const Books::Modification &modification, int line_num) {
  Book old_book;
  ReadBook(old_book, line_num);
  Book new_book(old_book);
  if (!modification.ISBN_.Empty()) {
    new_book.ISBN_ = modification.ISBN_;
    ISBN_map_.Delete(old_book.ISBN_, line_num);
    ISBN_map_.Insert(new_book.ISBN_, line_num);
  }
  if (!modification.book_name_.Empty()) {
    new_book.book_name_ = modification.book_name_;
    book_name_map_.Delete(old_book.book_name_, line_num);
    book_name_map_.Insert(new_book.book_name_, line_num);
  }
  if (!modification.author_.Empty()) {
    new_book.author_ = modification.author_;
    author_map_.Delete(old_book.author_, line_num);
    author_map_.Insert(new_book.author_, line_num);
  }
  if (!modification.keyword_.Empty()) {
    new_book.keyword_ = modification.keyword_;
    new_book.keywords_ = ParseKeywords(modification.keyword_.ToString());
    for (const auto &word : old_book.keywords_) {
      keyword_map_.Delete(word, line_num);
    }
    for (const auto &word : new_book.keywords_) {
      keyword_map_.Insert(word, line_num);
    }
  }
  if (!modification.quantity_.Empty()) {
    new_book.quantity_ = modification.quantity_;
  }
  if (!modification.price_.Empty()) {
    new_book.price_ = modification.price_;
  }
  WriteBook(new_book, line_num);
}
void Books::ReadBook(Book &dst, int line_num) {
  book_file_.Read(dst, line_num);
}
int Books::WriteBook(Book &src, int line_num) {
  if (line_num == -1) {
    book_file_.Write(src, book_cnt_);
    return book_cnt_++;
  }
  book_file_.Write(src, line_num);
  return line_num;
}