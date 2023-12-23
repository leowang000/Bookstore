#include "BookStore.h"

BookStore::BookStore(char *account_data_file_name, char *account_node_file_name, char *book_file_name,
                     char *ISBN_data_file_name, char *ISBN_node_file_name, char *book_name_data_file_name,
                     char *book_name_node_file_name, char *author_data_file_name, char *author_node_file_name,
                     char *keyword_data_file_name, char *keyword_node_file_name, char *finance_file_name,
                     char *log_file_name, char *employee_data_file_name, char *employee_node_file_name)
    : inst_(nullptr), accounts_(account_data_file_name, account_node_file_name),
    books_(book_file_name, ISBN_data_file_name, ISBN_node_file_name, book_name_data_file_name, book_name_node_file_name,
           author_data_file_name, author_node_file_name, keyword_data_file_name, keyword_node_file_name),
    log_(finance_file_name, log_file_name, employee_data_file_name, employee_node_file_name) {
  File log_file(log_file_name);
  if (log_file.Open()) {
    int i;
    log_file.seekg(-sizeof(log_info_t), std::ios::end);
    log_info_t last_log_info;
    log_file.read(reinterpret_cast<char *>(&last_log_info), sizeof(log_info_t));
    std::string time_string = last_log_info.ToString().substr(67, 6);
    time_ = 0;
    for (i = 0; i < 6; i++) {
      if (!isdigit(time_string[i])) {
        break;
      }
      time_ = 10 * time_ + time_string[i] - '0';
    }
    time_++;
    log_file.close();
  }
  else {
    time_ = 0;
  }
}
void BookStore::Init() {
  accounts_.Init();
}
void BookStore::GetInstruction(const std::string &in) {
  std::istringstream input(in);
  SkipSpaces(input);
  if (IsEndOfLine(input)) {
    inst_ = new Instruction(time_);
    return;
  }
  try {
    std::string op = ReadString(input, false, true, false, false, false);
    if (op == "quit" || op == "exit") {
      if (!IsEndOfLine(input)) {
        throw ErrorException("INVALID INPUT FORMAT");
      }
      inst_ = new QuitInst(time_, op);
    }
    else if (op == "su") {
      std::string user_id(ReadString(input, true, true, true, false, false));
      std::string password(IsEndOfLine(input) ? "" : ReadString(input, true, true, true, false, false, true));
      inst_ = new SuInst(time_, user_id, password);
    }
    else if (op == "logout") {
      if (!IsEndOfLine(input)) {
        throw ErrorException("INVALID INPUT FORMAT");
      }
      inst_ = new LogoutInst(time_);
    }
    else if (op == "register") {
      std::string user_id(ReadString(input, true, true, true, false, false));
      std::string password(ReadString(input, true, true, true, false, false));
      std::string username(ReadString(input, true, true, true, true, true, true));
      inst_ = new RegisterInst(time_, user_id, password, username);
    }
    else if (op == "passwd") {
      std::string user_id(ReadString(input, true, true, true, false, false));
      std::string password(ReadString(input, true, true, true, false, false));
      if (IsEndOfLine(input)) {
        inst_ = new PasswdInst(time_, user_id, "", password);
      }
      else {
        std::string new_password(ReadString(input, true, true, true, false, false, true));
        inst_ = new PasswdInst(time_, user_id, password, new_password);
      }
    }
    else if (op == "useradd") {
      std::string user_id(ReadString(input, true, true, true, false, false));
      std::string password(ReadString(input, true, true, true, false, false));
      std::string privilege(ReadString(input, true, false, false, false, false));
      std::string username(ReadString(input, true, true, true, true, true, true));
      inst_ = new UserAddInst(time_, user_id, password, privilege, username);
    }
    else if (op == "delete") {
      std::string user_id(ReadString(input, true, true, true, false, false, true));
      inst_ = new DeleteInst(time_, user_id);
    }
    else if (op == "show") {
      std::string tmp = ReadString(input, true, true, true, true, true, false, false, true, true);
      if (tmp == "finance") {
        if (IsEndOfLine(input)) {
          inst_ = new ShowFinanceInst(time_, "");
        }
        else {
          std::string count(ReadString(input, true, false, false, false, false, true));
          inst_ = new ShowFinanceInst(time_, count);
        }
      }
      else {
        if (!IsEndOfLine(input)) {
          throw ErrorException("INVALID INPUT FORMAT");
        }
        std::stringstream info(tmp);
        Books::Book show_index("", "", "", "", "", "");
        char buffer[10];
        if (!tmp.empty()) {
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
            std::string keyword = ReadString(info, true, true, true, false, true, true, true);
            if (ParseKeywords(keyword).size() != 1) {
              throw ErrorException("INVALID INPUT FORMAT");
            }
            show_index.keyword_ = keyword;
          }
          else {
            throw ErrorException("INVALID INPUT FORMAT");
          }
        }
        inst_ = new ShowInst(time_, show_index);
      }
    }
    else if (op == "buy") {
      std::string ISBN(ReadString(input, true, true, true, true, true));
      std::string quantity(ReadString(input, true, false, false, false, false, true));
      quantity_input_t test(quantity);
      inst_ = new BuyInst(time_, ISBN, quantity);
    }
    else if (op == "select") {
      std::string ISBN(ReadString(input, true, true, true, true, true, true));
      inst_ = new SelectInst(time_, ISBN);
    }
    else if (op == "modify") {
      Books::Book modification("", "", "", "", "", "");
      char buffer[10];
      if (IsEndOfLine(input)) {
        throw ErrorException("INVALID INPUT FORMAT");
      }
      while (!IsEndOfLine(input)) {
        std::string modification_info = ReadString(input, true, true, true, true, true);
        std::stringstream info(modification_info);
        std::string str__=info.str();
        if (modification_info.substr(0, 6) == "-ISBN=") {
          if (!modification.ISBN_.Empty()) {
            throw ErrorException("INVALID INPUT FORMAT");
          }
          info.read(buffer, 6);
          modification.ISBN_ = ReadString(info, true, true, true, true, true, true);
        }
        else if (modification_info.substr(0, 6) == "-name=") {
          if (!modification.book_name_.Empty()) {
            throw ErrorException("INVALID INPUT FORMAT");
          }
          info.read(buffer, 6);
          modification.book_name_ = ReadString(info, true, true, true, false, true, true, true);
        }
        else if (modification_info.substr(0, 8) == "-author=") {
          if (!modification.author_.Empty()) {
            throw ErrorException("INVALID INPUT FORMAT");
          }
          info.read(buffer, 8);
          modification.author_ = ReadString(info, true, true, true, false, true, true, true);
        }
        else if (modification_info.substr(0, 9) == "-keyword=") {
          if (!modification.keyword_.Empty()) {
            throw ErrorException("INVALID INPUT FORMAT");
          }
          info.read(buffer, 9);
          std::string keyword(ReadString(info, true, true, true, false, true, true, true));
          modification.keyword_ = keyword;
        }
        else if (modification_info.substr(0, 7) == "-price=") {
          if (!modification.price_.Empty()) {
            throw ErrorException("INVALID INPUT FORMAT");
          }
          info.read(buffer, 7);
          modification.price_ = ReadDouble(info);
        }
        else {
          throw ErrorException("INVALID INPUT FORMAT");
        }
      }
      inst_ = new ModifyInst(time_, modification);
    }
    else if (op == "import") {
      std::string quantity(ReadString(input, true, false, false, false, false));
      std::string cost(ReadDouble(input, true));
      quantity_input_t test(quantity);
      inst_ = new ImportInst(time_, quantity, cost);
    }
    else if (op == "report") {
      std::string tmp(ReadString(input, false, true, false, false, false, true));
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
      if (!IsEndOfLine(input)) {
        throw ErrorException("INVALID INPUT FORMAT");
      }
      inst_ = new LogInst(time_);
    }
    else {
      throw ErrorException("INVALID INPUT FORMAT");
    }
  }
  catch (ErrorException &ex) {
    throw ErrorException("INVALID INPUT FORMAT");
  }
}
void BookStore::CheckInstruction() {
  try {
    inst_->CheckParameter(accounts_, books_, log_);
    inst_->CheckPrivilege(accounts_);
  }
  catch (ErrorException &ex) {
    delete inst_;
    throw ErrorException(ex.GetMessage());
  }
}
bool BookStore::ExecuteInstruction() {
  bool will_exit = inst_->Execute(accounts_, books_, log_);
  delete inst_;
  return will_exit;
}
void BookStore::AddLog(bool valid) {
  user_t now_user_str(accounts_.GetNowUserString());
  if (valid) {
    instruction_t instruction_str(inst_->GetString());
    log_info_t log_info(now_user_str.GetString() + ": " + instruction_str.GetString());
    log_.AddLogInfo(log_info);
    if (accounts_.GetNowUserPrivilege() == 3) {
      log_.AddEmployeeInfo(now_user_str, instruction_str);
    }
    time_++;
    return;
  }
  instruction_t instruction_str(system_time_t(time_).GetString() + " INVALID");
  log_info_t log_info(now_user_str.GetString() + ": " + instruction_str.GetString());
  log_.AddLogInfo(log_info);
  time_++;
}