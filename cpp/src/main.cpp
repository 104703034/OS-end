
#include "banker.h"
#include "client.h"
#include "ui.h"

#include <chrono>
#include <thread>
#include <random>
#include <iostream>

int main(int, char**)
{
    int capital = 1234;
    auto bs = new BankState(capital);
    auto bkr = new Banker(capital);
    auto ec = new EventCollector(bs);
    auto spawn_func = [&]() {
      using namespace std::chrono_literals;
      using namespace std::chrono;
      std::default_random_engine generator;
      std::poisson_distribution<int> distribution(0.16);
      while(true) {
        std::this_thread::sleep_for(100ms);
        int roll = distribution(generator);
        while(roll--) {
          auto c = new Client(bkr, 0, ec);
          if(c->get_token() > 0) {
            bs->set_token_table(c->get_token(), c);
          }
          else
            delete c;
        }
      }
    };
    std::thread spawner(spawn_func);
    return ui_loop(bs);
}

