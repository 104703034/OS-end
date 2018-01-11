
#pragma once

#include "bankState.h"

#include <mutex>

class EventCollector {
 public:
  EventCollector(BankState* bs);
  void append_RESET(int);
  void append_NEW_CLIENT(int, int);
  void append_LEND(int, int);
  void append_CLIENT_EXIT(int);
 private:
  std::mutex mtx;
  BankState* bs;
};

