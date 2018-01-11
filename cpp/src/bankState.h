
#pragma once

#include <map>
#include <utility>
#include <mutex>

class Client; // decl here, to avoid circular dependencies

class BankState {
 public:
  BankState(int capital);
  void insertClient(int max_amount, int token);
  void deleteClient(int token);
  void clientBorrow(int token, int amount);
  const std::map<int, std::pair<int, int>>& get_client_money();
  const std::pair<int, int>& get_banker_money();
  void set_token_table(int token, Client* c);
  void set_token_mu(int token, double mu);
  double get_token_mu(int token);
  void lock_state();
  void unlock_state();
 private:
  std::map<int, std::pair<int, int>> client_money;
  std::pair<int, int> banker_money;
  std::recursive_mutex mtx;
  std::map<int, Client*> token_table;
};

