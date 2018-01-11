
#include "client.h"
#include "banker.h"
#include "eventCollector.h"
#include "bankState.h"

#include <cstdio>
#include <iostream>
#include <chrono>
#include <thread>
#include <random>
#include <cassert>
#include <algorithm>
#include <mutex>
#include <condition_variable>

using std::min;
using std::max;
using std::mutex;
using std::lock_guard;
using std::unique_lock;
using std::thread;

Client::Client(Banker* ib, double imu, EventCollector* iec):
  b(ib), mu(imu), should_terminate(false), borrowed_amount(0), ec(iec) {
  my_limit = (rand()*0.6/(RAND_MAX)) * b->get_capital();
  my_limit = max(my_limit, 1);
  bool ok = b->new_client(my_limit, &my_token);
  if(ok) {
    ec->append_NEW_CLIENT(my_limit, my_token);
    thr = new thread(&Client::run, this);
  }
  else
    my_token = -1;
}

Client::~Client() {
  should_terminate = true;
  if(my_token > 0) {
    thr->join();
    delete thr;
  }
}

void Client::set_mu(double m) {
  mu = m;
}

double Client::get_mu() const {
  return mu;
}

int Client::get_token() const {
  return my_token;
}

// main thread of Client
void Client::run() {
  using namespace std::chrono_literals;
  using namespace std::chrono;
  std::default_random_engine generator;
  double last_mu = -1;
  std::poisson_distribution<int> distribution;
  std::poisson_distribution<int> distribution_5(5);
  while(!should_terminate && borrowed_amount < my_limit) {
    std::this_thread::sleep_for(1000ms / fps);
    if(mu != last_mu) {
      distribution = std::poisson_distribution<int>(mu / fps);
      last_mu = mu;
    }
    int roll = distribution(generator);
    //using namespace std;
    //cout << "token " << my_token << " rolled " << roll << endl;
    for(int i = 0 ; i < roll ; i++) {
      int amount = min(distribution_5(generator)+1, my_limit-borrowed_amount);
      if(amount == 0)
        break; // all avaliable money have been borrowed
      if(b->lend(my_token, amount)) {
        borrowed_amount += amount;
        ec->append_LEND(my_token, amount);
      }
    }
  }
  std::this_thread::sleep_for(3s * (rand()*1.0/RAND_MAX));
  b->refundment(my_token);
  ec->append_CLIENT_EXIT(my_token);
}

