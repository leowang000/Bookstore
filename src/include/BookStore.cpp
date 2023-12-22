#include "BookStore.h"

BookStore::BookStore(char *account_data_file_name, char *account_node_file_name, char *book_file_name,
                     char *ISBN_data_file_name, char *ISBN_node_file_name, char *book_name_data_file_name,
                     char *book_name_node_file_name, char *author_data_file_name, char *author_node_file_name,
                     char *keyword_data_file_name, char *keyword_node_file_name, char *finance_file_name,
                     char *log_file_name, char *employee_data_file_name, char *employee_node_file_name)
    : time_(), inst_(nullptr), accounts_(account_data_file_name, account_node_file_name),
    books_(book_file_name, ISBN_data_file_name, ISBN_node_file_name, book_name_data_file_name, book_name_node_file_name,
           author_data_file_name, author_node_file_name, keyword_data_file_name, keyword_node_file_name),
    log_(finance_file_name, log_file_name, employee_data_file_name, employee_node_file_name) {}
void BookStore::GetInstruction() {
  SkipSpaces(std::cin);
  if (IsEndOfLine(std::cin)) {
    inst_ = new Instruction(time_);
    ReadLine(std::cin);
    return;
  }
  try {
    std::string op = ReadString(std::cin, false, true, false, false, false);
    if (op == "quit" || op == "exit") {
      if (!IsEndOfLine(std::cin)) {
        throw ErrorException("INVALID INPUT FORMAT");
      }
      inst_ = new QuitInst(time_, op);
    }
    else if (op == "su") {
      std::string user_id(ReadString(std::cin, true, true, true, false, false));
      std::string password(IsEndOfLine(std::cin) ? "" : ReadString(std::cin, true, true, true, false, false, true));
      inst_ = new SuInst(time_, user_id, password);
    }
    else if (op == "logout") {
      if (!IsEndOfLine(std::cin)) {
        throw ErrorException("INVALID INPUT FORMAT");
      }
      inst_ = new LogoutInst(time_);
    }
    else if (op == "register") {
      std::string user_id(ReadString(std::cin, true, true, true, false, false));
      std::string password(ReadString(std::cin, true, true, true, false, false));
      std::string username(ReadString(std::cin, true, true, true, true, true, true));
      inst_ = new RegisterInst(time_, user_id, password, username);
    }
    else if (op == "passwd") {
      std::string user_id(ReadString(std::cin, true, true, true, false, false));
      std::string password(ReadString(std::cin, true, true, true, false, false));
      if (IsEndOfLine(std::cin)) {
        inst_ = new PasswdInst(time_, user_id, "", password);
      }
      else {
        std::string new_password(ReadString(std::cin, true, true, true, false, false, true));
        inst_ = new PasswdInst(time_, user_id, password, new_password);
      }
    }
    else if (op == "useradd") {
      std::string user_id(ReadString(std::cin, true, true, true, false, false));
      std::string password(ReadString(std::cin, true, true, true, false, false));
      std::string privilege(ReadString(std::cin, true, false, false, false, false));
      std::string username(ReadString(std::cin, true, true, true, true, true, true));
      inst_ = new UserAddInst(time_, user_id, password, privilege, username);
    }
    else if (op == "delete") {
      std::string user_id(ReadString(std::cin, true, true, true, false, false, true));
      inst_ = new DeleteInst(time_, user_id);
    }
    else if (op == "show") {
      std::stringstream info(ReadString(std::cin, true, true, true, true, true, true));
      Books::Book show_index;
      char buffer[10];
      if (!IsEndOfLine(std::cin)) {
        if (info.str().substr(0, 6) == "-ISBN=") {
          info.read(buffer, 6);
          show_index.ISBN_ = ReadString(info, true, true, true, true, true, true);
        }
        else if (info.str().substr(0, 6) == "-name=") {
          info.read(buffer, 6);
          show_index.book_name_ = ReadString(info, true, true, true, false, true, true, true);
        }
        else if (info.str().substr(0, 8) == "-author=") {
          info.read(buffer, 8);
          show_index.author_ = ReadString(info, true, true, true, false, true, true, true);
        }
        else if (info.str().substr(0, 9) == "-keyword=") {
          info.read(buffer, 9);
          show_index.keyword_ = ReadString(info, true, true, true, false, true, true, true);
        }
        else {
          throw ErrorException("INVALID INPUT FORMAT");
        }
      }
      inst_ = new ShowInst(time_, show_index);
    }
    else if (op == "buy") {
      std::string ISBN(ReadString(std::cin, true, true, true, true, true));
      std::string quantity(ReadString(std::cin, true, false, false, false, false, true));
      quantity_input_t test(quantity);
      inst_ = new BuyInst(time_, ISBN, quantity);
    }
    else if (op == "select") {
      std::string ISBN(ReadString(std::cin, true, true, true, true, true, true));
      inst_ = new SelectInst(time_, ISBN);
    }
    else if (op == "modify") {
      Books::Book modification;
      char buffer[10];
      if (IsEndOfLine(std::cin)) {
        throw ErrorException("INVALID INPUT FORMAT");
      }
      while (!IsEndOfLine(std::cin)) {
        std::stringstream info(ReadString(std::cin, true, true, true, true, true, true));
        if (info.str().substr(0, 6) == "-ISBN=") {
          if (!modification.ISBN_.Empty()) {
            throw ErrorException("INVALID INPUT FORMAT");
          }
          info.read(buffer, 6);
          modification.ISBN_ = ReadString(info, true, true, true, true, true, true);
        }
        else if (info.str().substr(0, 6) == "-name=") {
          if (!modification.book_name_.Empty()) {
            throw ErrorException("INVALID INPUT FORMAT");
          }
          info.read(buffer, 6);
          modification.book_name_ = ReadString(info, true, true, true, false, true, true, true);
        }
        else if (info.str().substr(0, 8) == "-author=") {
          if (!modification.author_.Empty()) {
            throw ErrorException("INVALID INPUT FORMAT");
          }
          info.read(buffer, 8);
          modification.author_ = ReadString(info, true, true, true, false, true, true, true);
        }
        else if (info.str().substr(0, 9) == "-keyword=") {
          if (!modification.keyword_.Empty()) {
            throw ErrorException("INVALID INPUT FORMAT");
          }
          info.read(buffer, 9);
          std::string keyword(ReadString(info, true, true, true, false, true, true, true));
          modification.keyword_ = keyword;
          modification.keywords_ = ParseKeywords(keyword);
        }
        else {
          throw ErrorException("INVALID INPUT FORMAT");
        }
      }
      inst_ = new ModifyInst(time_, modification);
    }
    else if (op == "import") {
      std::string quantity(ReadString(std::cin, true, false, false, false, false));
      std::string cost(ReadDouble(std::cin, true));
      quantity_input_t test(quantity);
      inst_ = new ImportInst(time_, quantity, cost);
    }
    else if (op == "show") {
      if (!IsEndOfLine(std::cin)) {
        throw ErrorException("INVALID INPUT FORMAT");
      }
      std::string tmp(ReadString(std::cin, false, true, false, false, false));
      if (tmp != "finance") {
        throw ErrorException("INVALID INPUT FORMAT");
      }
      if (IsEndOfLine(std::cin)) {
        inst_ = new ShowFinanceInst(time_, "");
      }
      else {
        std::string count(ReadString(std::cin, true, false, false, false, false, true));
        inst_ = new ShowFinanceInst(time_, count);
      }
    }
    else if (op == "report") {
      std::string tmp(ReadString(std::cin, false, true, false, false, false, true));
      if (tmp == "finance") {
        inst_ = new ReportFinanceInst(time_);
      }
      else if (tmp == "employee") {
        inst_ = new ReportEmployeeInst(time_);
      }
      else {
        throw ErrorException("INVALID INPUT FORMAT");
      }
    }
    else if (op == "log") {
      if (!IsEndOfLine(std::cin)) {
        throw ErrorException("INVALID INPUT FORMAT");
      }
      inst_ = new LogInst(time_);
    }
    else {
      throw ErrorException("INVALID INPUT FORMAT");
    }
    ReadLine(std::cin);
  }
  catch (ErrorException &ex) {
    ReadLine(std::cin);
    throw ErrorException("INVALID INPUT FORMAT");
  }
}
void BookStore::CheckInstruction() {
  inst_->CheckParameter(accounts_, books_, log_);
  inst_->CheckPrivilege(accounts_);
}
void BookStore::ExecuteInstruction() {
  user_t now_user_str = accounts_.GetNowUserString();
  instruction_t instruction_str = inst_->GetString();
  instruction_str.Write(time_, -6);
  log_info_t log_info(now_user_str.GetString() + instruction_str.GetString());
  log_.AddLogInfo(log_info);
  if (accounts_.GetNowUserPrivilege() == 3) {
    log_.AddEmployeeInfo(now_user_str, instruction_str);
  }
  inst_->Execute(accounts_, books_, log_);
}