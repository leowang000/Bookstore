#include <iomanip>

#include "Log.h"
#include "BookStore.h"

FinanceInfo::FinanceInfo() : time_(), income_(), outcome_() {}
FinanceInfo::FinanceInfo(int time, const long double &income, const long double &outcome)
    : time_(time), income_(income), outcome_(outcome) {}
Log::Log(char *finance_file_name, char *log_file_name, char *employee_data_file_name, char *employee_node_file_name)
    : log_file_length_(), finance_file_length_(), finance_file_(finance_file_name), log_file_(log_file_name),
    employee_map_(employee_data_file_name, employee_node_file_name) {}
void Log::PrintLogFile(std::ostream &os) {
  int i, j;
  log_info_t info[logReadLength];
  os << "------------LOG------------\n";
  log_file_.Open();
  for (i = 0; i < log_file_length_; i += logReadLength) {
    if (i + logReadLength <= log_file_length_) {
      log_file_.read(reinterpret_cast<char *>(info), sizeof(log_info_t) * logReadLength);
      for (j = 0; j < logReadLength; j++) {
        os << info[j];
      }
      continue;
    }
    log_file_.read(reinterpret_cast<char *>(info), sizeof(log_info_t) * (log_file_length_ - i));
    for (j = 0; j < log_file_length_ - i; j++) {
      os << info[j];
    }
  }
  log_file_.close();
  os << "----------LOG END----------\n";
}//+ [收入] - [支出]\n
void Log::PrintFinanceInfo(std::ostream &os, int count) {
  FinanceInfo end, beg;
  if (count == -1) {
    if (finance_file_length_ == 0) {
      os << "+ 0.00 - 0.00\n";
      return;
    }
    finance_file_.Read(end, finance_file_length_ - 1);
    os << std::fixed << std::setprecision(2) << "+ " << end.income_ << " - " << end.outcome_ << "\n";
    return;
  }
  if (count == 0) {
    os << "\n";
    return;
  }
  finance_file_.Read(beg, finance_file_length_ - count);
  finance_file_.Read(end, finance_file_length_ - 1);
  os << std::fixed << std::setprecision(2) << "+ " << end.income_ - beg.income_ << " - " <<
      end.outcome_ - beg.outcome_ << "\n";
}
void Log::PrintFinance(std::ostream &os) {
  int i, j;
  FinanceInfo info[financeReadlength];
  long double prev_income = 0, prev_outcome = 0;
  os << "------------FINANCE------------\n";
  finance_file_.Open();
  for (i = 0; i < finance_file_length_; i += financeReadlength) {
    if (i + financeReadlength <= finance_file_length_) {
      finance_file_.read(reinterpret_cast<char *>(info), sizeof(FinanceInfo) * financeReadlength);
      for (j = 0; j < financeReadlength; j++) {
        os << std::fixed << std::setprecision(2) << "+ " << info[j].income_ - prev_income << " - " <<
            info[j].outcome_ - prev_outcome << " time :" << info[j].time_ << "\n";
        prev_income = info[j].income_;
        prev_outcome = info[j].outcome_;
      }
      continue;
    }
    finance_file_.read(reinterpret_cast<char *>(info), sizeof(FinanceInfo) * (finance_file_length_ - i));
    for (j = 0; j < finance_file_length_ - i; j++) {
      os << std::fixed << std::setprecision(2) << "+ " << info[j].income_ - prev_income << " - " <<
          info[j].outcome_ - prev_outcome << " time :" << info[j].time_ << "\n";
      prev_income = info[j].income_;
      prev_outcome = info[j].outcome_;
    }
  }
  finance_file_.close();
  os << "----------FINANCE END----------\n";
}
void Log::AddFinanceInfo(FinanceInfo &info) {
  finance_file_.Open();
  finance_file_.seekp(0, std::ios::end);
  finance_file_.write(reinterpret_cast<char *>(&info), sizeof(FinanceInfo));
  finance_file_.close();
  finance_file_length_++;
}
void Log::AddLogInfo(Log::log_info_t &info) {
  log_file_.Open();
  log_file_.seekp(0, std::ios::end);
  log_file_.write(reinterpret_cast<char *>(&info), sizeof(log_info_t));
  log_file_.close();
  log_file_length_++;
}
int Log::GetFinanceFileLength() const {
  return finance_file_length_;
}