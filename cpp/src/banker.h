
#pragma once

#include <map>
#include <set>
#include <mutex>

class Banker {
 public:
  Banker(int capital);
  int get_capital() const;
  int get_balance() const;
  bool new_client(int max_amount, int* token);
  bool lend(int token, int amount);
  void refundment(int token);
 private:
  const int capital; // amount of money banker initially holds
  int balance;
  std::map<int, int> lent_amount;
  std::map<int, int> client_limit;
  std::set<int> issued_tokens;
  bool is_safe();
  std::mutex mtx;
  int find_unused_token();
  static const int num_client_limit = 20;
};

