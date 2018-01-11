
#include "banker.h"

#include <map>
#include <set>
#include <mutex>
#include <cassert>
#include <iostream>
#include <vector>
#include <algorithm>

using std::map;
using std::set;
using std::lock_guard;
using std::mutex;

Banker::Banker(int c): capital(c), balance(capital) { }

int Banker::get_balance() const {
  return balance;
}

int Banker::get_capital() const {
  return capital;
}

bool Banker::new_client(int max_amount, int* token) {
  lock_guard<mutex> lck (mtx);
  assert(max_amount > 0);
  assert(max_amount <= capital);
  if(issued_tokens.size() >= num_client_limit)
    return false;
  int t = find_unused_token();
  *token = t;
  lent_amount[t] = 0;
  client_limit[t] = max_amount;
  assert(!issued_tokens.count(t));
  issued_tokens.insert(t);
  return true;
}

bool Banker::lend(int token, int amount) {
  lock_guard<mutex> lck (mtx);
  assert(amount > 0);
  assert(issued_tokens.count(token));
  assert(amount + lent_amount[token] <= client_limit[token]);
  if(balance < amount)
    return false;
  lent_amount[token] += amount;
  balance -= amount;
  if(is_safe())
    return true;
  else {
    lent_amount[token] -= amount;
    balance += amount;
    return false;
  }
}

void Banker::refundment(int token) {
  lock_guard<mutex> lck (mtx);
  assert(issued_tokens.count(token));
  balance += lent_amount[token];
  issued_tokens.erase(token);
  lent_amount.erase(token);
  client_limit.erase(token);
}

bool Banker::is_safe() {
  int b = balance;
  bool cont;
  map<int, bool> done;
  do {
    cont = false;
    for(const auto t: issued_tokens) {
      if(!done[t] && b >= client_limit[t] - lent_amount[t]) {
        b += lent_amount[t];
        done[t] = true;
        cont = true;
      }
    }
  } while(cont);
  return b == capital;
}

int Banker::find_unused_token() {
  using namespace std;
  vector<int> v;
  for(int i = 1 ; i <= num_client_limit ; i++)
    v.push_back(i);
  random_shuffle(v.begin(), v.end());
  for(auto i: v)
    if(!issued_tokens.count(i))
      return i;
  return -1;
}

