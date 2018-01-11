
#pragma once

#include "banker.h"
#include "eventCollector.h"

#include <thread>
#include <memory>
#include <mutex>
#include <condition_variable>

class Client {
 public:
  Client(Banker*, double, EventCollector*);
  ~Client();
  void set_mu(double);
  double get_mu() const;
  int get_token() const;
 private:
  Banker* b;
  std::thread* thr;
  void run();
  static const int fps = 30;
  int my_token;
  int my_limit;
  double mu;
  bool should_terminate;
  int borrowed_amount;
  EventCollector* ec;
};

