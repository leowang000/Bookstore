#include "Books.h"
#include "Tools/read_string.h"

Line::Line() : line_num_(), ISBN_() {}
Line::Line(int line_num, const ISBN_t &ISBN) : line_num_(line_num), ISBN_(ISBN) {}
bool Line::operator<(const Line &rhs) const {
  return ISBN_ < rhs.ISBN_;
}
bool Line::operator==(const Line &rhs) const {
  return ISBN_ == rhs.ISBN_;
}
bool Line::operator>(const Line &rhs) const {
  return ISBN_ > rhs.ISBN_;
}
bool Line::operator<=(const Line &rhs) const {
  return ISBN_ <= rhs.ISBN_;
}
bool Line::operator>=(const Line &rhs) const {
  return ISBN_ >= rhs.ISBN_;
}
bool Line::operator!=(const Line &rhs) const {
  return ISBN_ != rhs.ISBN_;
}
Books::Book::Book(const std::string &ISBN, const std::string &book_name, const std::string &author,
                  const std::string &keyword, const std::string &quantity, const std::string &price)
    : ISBN_(ISBN), book_name_(book_name), author_(author), keyword_(keyword), quantity_(quantity), price_(price) {}
std::string Books::Book::GetString() {
  return ISBN_.GetString(-1, false) + "\t" + book_name_.GetString(-1, false) + "\t" + author_.GetString(-1, false) +
  "\t" + keyword_.GetString(-1, false) + "\t" +  price_.GetString(2, false) + "\t" + quantity_.GetString(-1, false);
}
Books::Books(char *book_file_name, char *ISBN_data_file_name, char *ISBN_node_file_name,
             char *book_name_data_file_name, char *book_name_node_file_name, char *author_data_file_name,
             char *author_node_file_name, char *keyword_data_file_name, char *keyword_node_file_name)
    : book_file_(book_file_name), ISBN_map_(ISBN_data_file_name, ISBN_node_file_name),
    book_name_map_(book_name_data_file_name, book_name_node_file_name),
    author_map_(author_data_file_name, author_node_file_name),
    keyword_map_(keyword_data_file_name, keyword_node_file_name) {
  book_cnt_ = ISBN_map_.Length();
}
std::vector<int> Books::FindByISBN(const ISBN_t &ISBN) {
  return ISBN_map_.Find(ISBN);
}
std::vector<int> Books::FindByBookName(const book_name_t &book_name) {
  std::vector<Line> lines(book_name_map_.Find(book_name));
  std::vector<int> result;
  for (auto &line : lines) {
    result.push_back(line.line_num_);
  }
  return result;
}
std::vector<int> Books::FindByAuthor(const author_t &author) {
  std::vector<Line> lines(author_map_.Find(author));
  std::vector<int> result;
  for (auto &line : lines) {
    result.push_back(line.line_num_);
  }
  return result;
}
std::vector<int> Books::FindByKeyword(const keyword_t &keyword) {
  std::vector<Line> lines(keyword_map_.Find(keyword));
  std::vector<int> result;
  for (auto &line : lines) {
    result.push_back(line.line_num_);
  }
  return result;
}
bool Books::HaveISBN(const ISBN_t &ISBN) {
  return ISBN_map_.Have(ISBN);
}
void Books::Modify(Books::Book &modification, int line_num) {
  Book old_book;
  ReadBook(old_book, line_num);
  Line old_line(line_num, old_book.ISBN_);
  if (!modification.ISBN_.Empty()) {
    ISBN_map_.Delete(old_book.ISBN_, line_num);
    ISBN_map_.Insert(modification.ISBN_, line_num);
    old_book.ISBN_ = modification.ISBN_;
  }
  Line new_line(line_num, old_book.ISBN_);
  if (!modification.book_name_.Empty()) {
    book_name_map_.Delete(old_book.book_name_, old_line);
    book_name_map_.Insert(modification.book_name_, new_line);
    old_book.book_name_ = modification.book_name_;
  }
  if (!modification.author_.Empty()) {
    author_map_.Delete(old_book.author_, old_line);
    author_map_.Insert(modification.author_, new_line);
    old_book.author_ = modification.author_;
  }
  if (!modification.keyword_.Empty()) {
    std::vector<std::string> old_keywords(ParseKeywords(old_book.keyword_.GetString(-1, false)));
    std::vector<std::string> new_keywords(ParseKeywords(modification.keyword_.GetString(-1, false)));
    for (const auto &word : old_keywords) {
      keyword_map_.Delete(word, old_line);
    }
    for (const auto &word : new_keywords) {
      keyword_map_.Insert(word, new_line);
    }
    old_book.keyword_ = modification.keyword_;
  }
  if (!modification.price_.Empty()) {
    old_book.price_ = modification.price_;
  }
  if (!modification.quantity_.Empty()) {
    old_book.quantity_ = modification.quantity_;
  }
  WriteBook(old_book, line_num);
}
int Books::AddBook(Books::Book &book) {
  int line_num = WriteBook(book);
  Line line(line_num, book.ISBN_);
  ISBN_map_.Insert(book.ISBN_, line_num);
  book_name_map_.Insert(book.book_name_, line);
  author_map_.Insert(book.author_, line);
  std::vector<std::string> keywords = ParseKeywords(book.keyword_.GetString(-1, false));
  for (auto &word : keywords) {
    keyword_map_.Insert(word, line);
  }
  return line_num;
}
void Books::PrintBookFile(std::ostream &os) {
  std::vector<UnrolledLinkedList<ISBN_t, int>::pair_t> line_nums(ISBN_map_.GetAll());
  for (auto &pair : line_nums) {
    Book book;
    ReadBook(book, pair.val_);
    os << book.GetString() << "\n";
  }
}
void Books::PrintBooks(std::ostream &os, const std::vector<int> &line_nums) {
  if (line_nums.empty()) {
    os << "\n";
    return;
  }
  for (auto &line_num : line_nums) {
    Book book;
    ReadBook(book, line_num);
    os << book.GetString() << "\n";
  }
}
Books::Book Books::GetBook(const ISBN_t &ISBN) {
  Book book;
  ReadBook(book, FindByISBN(ISBN).front());
  return book;
}
Books::Book Books::GetBook(int line_num) {
  Book book;
  ReadBook(book, line_num);
  return book;
}
void Books::ReadBook(Book &dst, int line_num) {
  book_file_.Read(dst, line_num);
}
int Books::WriteBook(Book &src, int line_num) {
  if (line_num == -1) {
    line_num = book_cnt_;
    book_cnt_++;
  }
  book_file_.Write(src, line_num);
  return line_num;
}