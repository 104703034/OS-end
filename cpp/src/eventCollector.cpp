
#include "eventCollector.h"
#include "bankState.h"

#include <mutex>
#include <iostream>

using std::mutex;
using std::lock_guard;
using std::cout;
using std::endl;

EventCollector::EventCollector(BankState* ibs): bs(ibs) {}

void EventCollector::append_RESET(int capital) {
  lock_guard<mutex> lk(mtx);
  cout << "RESET " << capital << endl;
  auto old_bs = bs;
  bs = new BankState(capital);
  delete old_bs;
}
void EventCollector::append_NEW_CLIENT(int limit, int client_index) {
  lock_guard<mutex> lk(mtx);
  cout << "NEW_CLIENT " << limit << " " << client_index << endl;
  bs->insertClient(limit, client_index);
}
void EventCollector::append_LEND(int client_index, int amount) {
  lock_guard<mutex> lk(mtx);
  cout << "LEND " << client_index << " " << amount << endl;
  bs->clientBorrow(client_index, amount);
}
void EventCollector::append_CLIENT_EXIT(int client_index) {
  lock_guard<mutex> lk(mtx);
  cout << "CLIENT_EXIT " << client_index << endl;
  bs->deleteClient(client_index);
}

