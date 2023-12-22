#ifndef BOOKSTORE_2023_FILE_H
#define BOOKSTORE_2023_FILE_H

#include <fstream>

class File : public std::fstream {
public:
  explicit File(char *);

  bool Open(); // open or create
  template<class T> void Read(T &, int, int bias = 0);
  template<class T> void Write(T &, int, int bias = 0);

private:
  char *file_name_;
};

template<class T>
void File::Read(T &dst, int index, int bias) {
  Open();
  this->seekg(index * sizeof(T) + bias);
  this->read(reinterpret_cast<char *>(&dst), sizeof(T));
  this->close();
}
template<class T>
void File::Write(T &src, int index, int bias) {
  Open();
  this->seekp(index * sizeof(T) + bias);
  this->write(reinterpret_cast<char *>(&src), sizeof(T));
  this->close();
}

#endif //BOOKSTORE_2023_FILE_H