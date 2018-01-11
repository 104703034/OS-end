
#include "bankState.h"
#include "client.h"

#include <map>
#include <utility>
#include <mutex>
#include <cassert>

using std::map;
using std::pair;
using std::lock_guard;
using std::recursive_mutex;

BankState::BankState(int capital) {
  banker_money = {capital, capital};
}

void BankState::lock_state() {
  mtx.lock();
}
void BankState::unlock_state() {
  mtx.unlock();
}

void BankState::insertClient(int max_amount, int token) {
  lock_guard<recursive_mutex> lk(mtx);
  assert(client_money.find(token) == client_money.end());
  client_money[token] = {0, max_amount};
}

void BankState::deleteClient(int token) {
  lock_guard<recursive_mutex> lk(mtx);
  assert(client_money.find(token) != client_money.end());
  banker_money.first += client_money[token].first;
  client_money.erase(token);
}

void BankState::clientBorrow(int token, int amount) {
  lock_guard<recursive_mutex> lk(mtx);
  assert(client_money.find(token) != client_money.end());
  banker_money.first -= amount;
  client_money[token].first += amount;
}

const map<int, pair<int, int>>& BankState::get_client_money() {
  return client_money;
}

const pair<int, int>& BankState::get_banker_money() {
  return banker_money;
}

void BankState::set_token_table(int token, Client* c) {
  lock_guard<recursive_mutex> lk(mtx);
  auto iter = token_table.find(token);
  if(iter != token_table.end())
    delete (iter->second);
  token_table[token] = c;
}

double BankState::get_token_mu(int token) {
  lock_guard<recursive_mutex> lk(mtx);
  auto iter = token_table.find(token);
  assert(iter != token_table.end());
  return iter->second->get_mu();
}

void BankState::set_token_mu(int token, double mu) {
  lock_guard<recursive_mutex> lk(mtx);
  auto iter = token_table.find(token);
  if(iter != token_table.end()) // token_table may have not been updated
    iter->second->set_mu(mu);
}


