#include "pool.h"
#include <iostream>
#include <memory>

Pool::Pool(std::shared_ptr<std::mutex> m,
           std::shared_ptr<std::condition_variable> &con)
    : condition{con}, Queue_Mutex{m} {}

void Pool::work(unsigned &co) {
  job Job = nullptr;
  while (true) {
    {
      std::unique_lock<std::mutex> lock{*Queue_Mutex};
      condition->wait(lock,
                      [this]() { return !Queue.empty() || terminate_pool; });
      if (!Queue.empty() && !terminate_pool) {
        Job = Queue.front();
        Queue.pop();
      } else if (terminate_pool) {
        break;
      }
    }
    // std::invoke(Job); // function<void()> type
    if (Job) {
      Job(co);
      std::cout << __FUNCTION__ << " did job " << co << std::endl;
      Job = nullptr;
    }
    condition->notify_one();
  }
}

void Pool::add_job(job j) {
  {
    std::lock_guard<std::mutex> lock{*Queue_Mutex};
    Queue.push(j);
    terminate_pool = false;
  }
  condition->notify_one();
}

void Pool::shutdown() {
  {
    std::lock_guard<std::mutex> lock{*Queue_Mutex};
    terminate_pool = true;
  }
  condition->notify_all();
}
