// implementation of unrolled linked list
#ifndef BOOKSTORE_2023_UNROLLEDLINKEDLIST_H
#define BOOKSTORE_2023_UNROLLEDLINKEDLIST_H
#include <iostream>
#include <cstring>
#include <algorithm>
#include <fstream>
#include <queue>
#include <list>
#include <limits>

template<int len> class String;
template<int len> std::ostream &operator<<(std::ostream &, const String<len> &);
template<int len> std::istream &operator>>(std::istream &, String<len> &);
template<int len>
class String {
private:
  char str_[len + 1];
public:
  String();
  String(const String<len> &);
  String(const char *other);
  String(const std::string &other);
  String<len> &operator=(const String<len> &);

  char &operator[](int);
  bool operator<(const String<len> &) const;
  bool operator>(const String<len> &) const;
  bool operator<=(const String<len> &) const;
  bool operator==(const String<len> &) const;
  bool operator>=(const String<len> &) const;
  bool operator!=(const String<len> &) const;
  friend std::ostream &operator<<<len>(std::ostream &, const String<len> &);
  friend std::istream &operator>><len>(std::istream &, String<len> &);
};
template<class key_t, class val_t>
class UnrolledLinkedList {
private:
  static const int MAX_BLOCK_SIZE = 300;
  static const int MIN_BLOCK_SIZE = 5;
  static const int MAX_BLOCK_INDEX = 10000;

  struct pair_t {
    key_t key_;
    val_t val_;

    pair_t();
    pair_t(key_t, val_t);
    pair_t(const pair_t &) = default;
    bool operator<(const pair_t &) const;
    bool operator>(const pair_t &) const;
    bool operator>=(const pair_t &) const;
    bool operator==(const pair_t &) const;
    bool operator<=(const pair_t &) const;
    bool operator!=(const pair_t &) const;
    pair_t &operator=(const pair_t &);
  };
  struct Node {
    pair_t min_pair_;
    int size_, block_index_;

    Node();
    Node(const pair_t &, int, int);
    Node(const Node &) = default;
    Node &operator=(const Node &);
  };

  using Block = std::vector<pair_t>;
  using Iter = typename std::list<Node>::iterator;

  std::priority_queue<int, std::vector<int>, std::greater<int>> unused_block_index_;
  std::list<Node> node_list_;
  char *data_file_name_, *node_file_name_;

  void InitDataFileAndUnusedBlockIndex();
  void WriteUnusedBlockIndex();
  void InitNodeList();
  void WriteNodeList();
  void ReadBlock(Block &, int);
  int WriteBlock(Block &, int block_index = -1);
  Iter FindInsertNode(const pair_t &);
  void Merge(Iter, Block &);
  void Split(Iter, Block &);

public:
  UnrolledLinkedList(char *, char *);
  ~UnrolledLinkedList();
  void Insert(key_t, val_t);
  void Delete(key_t, val_t);
  std::vector<val_t> Find(key_t);
};

template<int len>
String<len>::String() {
  int i;
  for (i = 0; i <= len; i++) {
    str_[i] = '\0';
  }
}
template<int len>
String<len>::String(const String<len> &other) {
  strncpy(str_, other.str_, len + 1);
}
template<int len>
String<len>::String(const char *other) { // strlen(other) <= len
  int i;
  strcpy(str_, other);
  for (i = strlen(other); i <= len; i++) {
    str_[i] = '\0';
  }
}
template<int len>
String<len>::String(const std::string &other) : String(other.c_str()) {} // other.length() <= len
template<int len>
String<len> &String<len>::operator=(const String<len> &other) {
  if (this == &other) {
    return *this;
  }
  strncpy(str_, other.str_, len + 1);
  return *this;
}
template<int len>
char &String<len>::operator[](int index) { // index >= 0 && index < len
  return str_[index];
}
template<int len>
bool String<len>::operator<(const String<len> &rhs) const {
  return strncmp(str_, rhs.str_, len + 1) < 0;
}
template<int len>
bool String<len>::operator>(const String<len> &rhs) const {
  return strncmp(str_, rhs.str_, len + 1) > 0;
}
template<int len>
bool String<len>::operator<=(const String<len> &rhs) const {
  return strncmp(str_, rhs.str_, len + 1) <= 0;
}
template<int len>
bool String<len>::operator==(const String<len> &rhs) const {
  return strncmp(str_, rhs.str_, len + 1) == 0;
}
template<int len>
bool String<len>::operator>=(const String<len> &rhs) const {
  return strncmp(str_, rhs.str_, len + 1) >= 0;
}
template<int len>
bool String<len>::operator!=(const String<len> &rhs) const {
  return strncmp(str_, rhs.str_, len + 1) != 0;
}
template<int len>
std::ostream &operator<<(std::ostream &os, const String<len> &str) {
  os << str.str_;
  return os;
}
template<int len>
std::istream &operator>>(std::istream &is, String<len> &str) {
  is >> str.str_;
  return is;
}
template<class key_t, class val_t>
UnrolledLinkedList<key_t, val_t>::pair_t::pair_t() : key_(), val_() {}
template<class key_t, class val_t>
UnrolledLinkedList<key_t, val_t>::pair_t::pair_t(key_t key, val_t val) : key_(key), val_(val) {}
template<class key_t, class val_t>
bool UnrolledLinkedList<key_t, val_t>::pair_t::operator<(const UnrolledLinkedList<key_t, val_t>::pair_t &rhs) const {
  if (key_ != rhs.key_) {
    return key_ < rhs.key_;
  }
  return val_ < rhs.val_;
}
template<class key_t, class val_t>
bool UnrolledLinkedList<key_t, val_t>::pair_t::operator>(const UnrolledLinkedList<key_t, val_t>::pair_t &rhs) const {
  if (key_ != rhs.key_) {
    return key_ > rhs.key_;
  }
  return val_ > rhs.val_;
}
template<class key_t, class val_t>
bool UnrolledLinkedList<key_t, val_t>::pair_t::operator>=(const UnrolledLinkedList<key_t, val_t>::pair_t &rhs) const {
  return key_ > rhs.key_ || (key_ == rhs.key_ && val_ >= rhs.val_);
}
template<class key_t, class val_t>
bool UnrolledLinkedList<key_t, val_t>::pair_t::operator==(const UnrolledLinkedList<key_t, val_t>::pair_t &rhs) const {
  return key_ == rhs.key_ && val_ == rhs.val_;
}
template<class key_t, class val_t>
bool UnrolledLinkedList<key_t, val_t>::pair_t::operator<=(const UnrolledLinkedList<key_t, val_t>::pair_t &rhs) const {
  return key_ < rhs.key_ || (key_ == rhs.key_ && val_ <= rhs.val_);
}
template<class key_t, class val_t>
bool UnrolledLinkedList<key_t, val_t>::pair_t::operator!=(const UnrolledLinkedList<key_t, val_t>::pair_t &rhs) const {
  return key_ != rhs.key_ || val_ != rhs.val_;
}
template<class key_t, class val_t>
typename UnrolledLinkedList<key_t, val_t>::pair_t &UnrolledLinkedList<key_t, val_t>::pair_t::operator=(
    const UnrolledLinkedList<key_t, val_t>::pair_t &rhs) {
  if (this == &rhs) {
    return *this;
  }
  key_ = rhs.key_;
  val_ = rhs.val_;
  return *this;
}
template<class key_t, class val_t>
UnrolledLinkedList<key_t, val_t>::Node::Node() : min_pair_(), size_(), block_index_() {}
template<class key_t, class val_t>
UnrolledLinkedList<key_t, val_t>::Node::Node(const pair_t &min_pair, int size, int block_index)
    : min_pair_(min_pair), size_(size), block_index_(block_index) {}
template<class key_t, class val_t>
typename UnrolledLinkedList<key_t, val_t>::Node &UnrolledLinkedList<key_t, val_t>::Node::operator=(
    const UnrolledLinkedList<key_t, val_t>::Node &rhs) {
  if (this == &rhs) {
    return *this;
  }
  min_pair_ = rhs.min_pair_;
  size_ = rhs.size_;
  block_index_ = rhs.block_index_;
  return *this;
}
template<class key_t, class val_t>
void UnrolledLinkedList<key_t, val_t>::InitDataFileAndUnusedBlockIndex() {
  int i, zero = 0, num;
  std::fstream data_file(data_file_name_);
  while (!unused_block_index_.empty()) {
    unused_block_index_.pop();
  }
  if (!data_file.good()) {
    data_file.open(data_file_name_, std::ios::out);
    data_file.close();
    data_file.open(data_file_name_);
    for (i = 0; i < MAX_BLOCK_INDEX; i++) {
      unused_block_index_.push(i);
    }
    data_file.seekp(0);
    for (i = 0; i < MAX_BLOCK_INDEX; i++) {
      data_file.write(reinterpret_cast<char *>(&zero), sizeof(int));
    }
    data_file.close();
    return;
  }
  data_file.seekg(0);
  for (i = 0; i < MAX_BLOCK_INDEX; i++) {
    data_file.read(reinterpret_cast<char *>(&num), sizeof(int));
    if (num) {
      unused_block_index_.push(i);
    }
  }
  data_file.close();
}
template<class key_t, class val_t>
void UnrolledLinkedList<key_t, val_t>::InitNodeList() {
  std::fstream node_file(node_file_name_);
  Node node;
  if (!node_file.good()){
    node_file.open(node_file_name_, std::ios::out);
    node_file.close();
    node_list_.clear();
    node_file.close();
    return;
  }
  while (node_file.read(reinterpret_cast<char *>(&node), sizeof(Node))) {
    if (node_file.eof()) {
      break;
    }
    std::vector<pair_t> block;
    ReadBlock(block, node.block_index_);
    node_list_.push_back(node);
  }
  node_file.close();
}
template<class key_t, class val_t>
void UnrolledLinkedList<key_t, val_t>::WriteUnusedBlockIndex() {
  int i, zero = 0, one = 1;
  std::fstream data_file(data_file_name_);
  data_file.seekp(0);
  for (i = 0; i < MAX_BLOCK_INDEX; i++) {
    data_file.write(reinterpret_cast<char *>(&zero), sizeof(int));
  }
  while (!unused_block_index_.empty()) {
    data_file.seekp(unused_block_index_.top() * sizeof(int));
    data_file.write(reinterpret_cast<char *>(&one), sizeof(int));
    unused_block_index_.pop();
  }
  data_file.close();
}
template<class key_t, class val_t>
void UnrolledLinkedList<key_t, val_t>::WriteNodeList() {
  std::fstream node_file(node_file_name_);
  for (auto &it : node_list_) {
    node_file.write(reinterpret_cast<char *>(&it), sizeof(Node));
  }
  node_file.close();
}
template<class key_t, class val_t>
void UnrolledLinkedList<key_t, val_t>::ReadBlock(Block &block, int block_index) {
  std::fstream data_file(data_file_name_, std::ios::in | std::ios::out |std::ios::binary);
  block.resize(MAX_BLOCK_SIZE);
  data_file.seekg(MAX_BLOCK_INDEX * sizeof(int) + block_index * MAX_BLOCK_SIZE * sizeof(pair_t));
  data_file.read(reinterpret_cast<char *>(block.data()), MAX_BLOCK_SIZE * sizeof(pair_t));
  data_file.close();
}
template<class key_t, class val_t>
int UnrolledLinkedList<key_t, val_t>::WriteBlock(Block &block, int block_index) {
  std::fstream data_file(data_file_name_, std::ios::in | std::ios::out |std::ios::binary);
  if (block_index == -1) {
    block_index = unused_block_index_.top();
    unused_block_index_.pop();
  }
  data_file.seekp(MAX_BLOCK_INDEX * sizeof(int) + block_index * MAX_BLOCK_SIZE * sizeof(pair_t));
  data_file.write(reinterpret_cast<char *>(block.data()), MAX_BLOCK_SIZE * sizeof(pair_t));
  data_file.close();
  return block_index;
}
template<class key_t, class val_t>
typename UnrolledLinkedList<key_t, val_t>::Iter UnrolledLinkedList<key_t, val_t>::FindInsertNode(const pair_t &pair) {
  Iter ans = node_list_.begin();
  if (pair < ans->min_pair_) {
    return ans;
  }
  while (ans != node_list_.end() && ans->min_pair_ < pair) {
    ++ans;
  }
  if (ans != node_list_.begin()) {
    --ans;
  }
  return ans;
}
template<class key_t, class val_t>
void UnrolledLinkedList<key_t, val_t>::Merge(Iter it, Block &block) {
  Block prev_block, now_block;
  int i;
  if (it->size_ == 0) {
    unused_block_index_.push(it->block_index_);
    node_list_.erase(it);
    return;
  }
  if (it == node_list_.begin()) {
    block.resize(MAX_BLOCK_SIZE);
    WriteBlock(block, it->block_index_);
    return;
  }
  Iter prev_it = std::prev(it);
  if (prev_it->size_ < it->size_) {
    block.resize(MAX_BLOCK_SIZE);
    WriteBlock(block, it->block_index_);
    return;
  }
  ReadBlock(prev_block, prev_it->block_index_);
  if (prev_it->size_ + block.size() <= MAX_BLOCK_SIZE) {
    for (i = 0; i < it->size_; i++) {
      prev_block[prev_it->size_] = block[i];
      prev_it->size_++;
    }
    WriteBlock(prev_block, prev_it->block_index_);
    unused_block_index_.push(it->block_index_);
    node_list_.erase(it);
  }
  else {
    for (i = (prev_it->size_ + it->size_) / 2; i < prev_it->size_; i++) {
      now_block.push_back(prev_block[i]);
    }
    for (i = 0; i < it->size_; i++) {
      now_block.push_back(block[i]);
    }
    prev_it->size_ = (prev_it->size_ + it->size_) / 2;
    it->size_ = now_block.size();
    it->min_pair_ = now_block.front();
    now_block.resize(MAX_BLOCK_SIZE);
    WriteBlock(prev_block, prev_it->block_index_);
    WriteBlock(now_block, it->block_index_);
  }
}
template<class key_t, class val_t>
void UnrolledLinkedList<key_t, val_t>::Split(Iter it, Block &block) {
  int i, block_copy_size;
  it->size_ /= 2;
  Block block_copy(block.begin() + it->size_, block.end());
  block_copy_size = block_copy.size();
  block_copy.resize(MAX_BLOCK_SIZE);
  block.resize(MAX_BLOCK_SIZE);
  for (i = it->size_; i < MAX_BLOCK_SIZE; i++) {
    block[i] = pair_t();
  }
  WriteBlock(block, it->block_index_);
  node_list_.emplace(std::next(it), block_copy.front(), block_copy_size, WriteBlock(block_copy));
}
template<class key_t, class val_t>
UnrolledLinkedList<key_t, val_t>::UnrolledLinkedList(char *data_file_name, char *node_file_name)
    : data_file_name_(data_file_name), node_file_name_(node_file_name) {
  InitDataFileAndUnusedBlockIndex();
  InitNodeList();
}
template<class key_t, class val_t>
UnrolledLinkedList<key_t, val_t>::~UnrolledLinkedList() {
  WriteNodeList();
  WriteUnusedBlockIndex();
}
template<class key_t, class val_t>
void UnrolledLinkedList<key_t, val_t>::Insert(key_t key, val_t value) {
  Block block;
  pair_t pair(key, value);
  if (node_list_.empty()) {
    block.resize(MAX_BLOCK_SIZE);
    block[0] = pair_t(key, value);
    node_list_.emplace_back(pair, 1, WriteBlock(block));
    return;
  }
  auto it = FindInsertNode(pair);
  if (it != --node_list_.end() && std::next(it)->min_pair_ == pair) {
    return;
  }
  ReadBlock(block, it->block_index_);
  auto pos = std::lower_bound(block.begin(), block.begin() + it->size_, pair);
  if (pos != block.end() && *pos == pair) {
    return;
  }
  block.insert(pos, pair);
  it->size_++;
  it->min_pair_ = block.front();
  if (it->size_ <= MAX_BLOCK_SIZE) {
    block.resize(MAX_BLOCK_SIZE);
    WriteBlock(block, it->block_index_);
  }
  else {
    Split(it, block);
  }
}
template<class key_t, class val_t>
void UnrolledLinkedList<key_t, val_t>::Delete(key_t key, val_t value) {
  Block block;
  pair_t pair(key, value);
  if (pair < node_list_.begin()->min_pair_) {
    return;
  }
  Iter it = FindInsertNode(pair);
  while (it != node_list_.end() && pair >= it->min_pair_) {
    auto next_iter = std::next(it);
    ReadBlock(block, it->block_index_);
    auto st_it = std::lower_bound(block.begin(), block.begin() + it->size_, pair);
    auto end_it = std::upper_bound(block.begin(), block.begin() + it->size_, pair);
    it->size_ -= (end_it - st_it);
    block.erase(st_it, end_it);
    if (it->size_ < MIN_BLOCK_SIZE) {
      Merge(it, block);
    }
    else {
      block.resize(MAX_BLOCK_SIZE);
      it->min_pair_ = block.front();
      WriteBlock(block, it->block_index_);
    }
    it = next_iter;
  }
}
template<class key_t, class val_t>
std::vector<val_t> UnrolledLinkedList<key_t, val_t>::Find(key_t key) {
  std::vector<val_t> ans;
  Block block;
  if (node_list_.empty() || key < node_list_.begin()->min_pair_.key_) {
    return ans;
  }
  pair_t min_pair(key, val_t()), max_pair(key, std::numeric_limits<val_t>::max());
  auto it = FindInsertNode(pair_t(key, 0));
  while (it != node_list_.end() && it->min_pair_.key_ <= key) {
    ReadBlock(block, it->block_index_);
    auto st_it = std::lower_bound(block.begin(), block.begin() + it->size_, min_pair);
    auto end_it = std::upper_bound(block.begin(), block.begin() + it->size_, max_pair);
    for (auto iter = st_it; iter != end_it; ++iter) {
      ans.push_back(iter->val_);
    }
    ++it;
  }
  return ans;
}
#endif // BOOKSTORE_2023_UNROLLEDLINKEDLIST_H