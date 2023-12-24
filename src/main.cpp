#include "include/BookStore.h"
#include <filesystem>

char account_data_file_name[] = "account_data";
char account_node_file_name[] = "account_node";
char book_file_name[] = "book";
char ISBN_data_file_name[] = "ISBN_data";
char ISBN_node_file_name[] = "ISBN_node";
char book_name_data_file_name[] = "book_name_data";
char book_name_node_file_name[] = "book_name_node";
char author_data_file_name[] = "author_data";
char author_node_file_name[] = "author_node";
char keyword_data_file_name[] = "keyword_data";
char keyword_node_file_name[] = "keyword_node";
char finance_file_name[] = "finance";
char log_file_name[] = "log";
char employee_data_file_name[] = "employee_data";
char employee_node_file_name[] = "employee_node";

void ClearFiles() {
  try {
    std::filesystem::remove(account_data_file_name);
    std::filesystem::remove(account_node_file_name);
    std::filesystem::remove(book_file_name);
    std::filesystem::remove(ISBN_data_file_name);
    std::filesystem::remove(ISBN_node_file_name);
    std::filesystem::remove(book_name_data_file_name);
    std::filesystem::remove(book_name_node_file_name);
    std::filesystem::remove(author_data_file_name);
    std::filesystem::remove(author_node_file_name);
    std::filesystem::remove(keyword_data_file_name);
    std::filesystem::remove(keyword_node_file_name);
    std::filesystem::remove(finance_file_name);
    std::filesystem::remove(log_file_name);
    std::filesystem::remove(employee_data_file_name);
    std::filesystem::remove(employee_node_file_name);
  }
  catch (...) {}
}
int main() {
  //ClearFiles();
  BookStore book_store(account_data_file_name, account_node_file_name, book_file_name, ISBN_data_file_name,
                      ISBN_node_file_name, book_name_data_file_name, book_name_node_file_name, author_data_file_name,
                      author_node_file_name, keyword_data_file_name, keyword_node_file_name, finance_file_name,
                      log_file_name, employee_data_file_name, employee_node_file_name);
  book_store.Init();
  while (true) {
    try {
      std::string input;
      getline(std::cin, input);
      if (!std::cin) {
        break;
      }
      book_store.GetInstruction(input);
      book_store.CheckInstruction();
      book_store.AddLog();
      if (book_store.ExecuteInstruction()) {
        break;
      }
    }
    catch (ErrorException &ex) {
      std::cout << "Invalid\n";
      book_store.AddLog();
    }
  }
  return 0;
}