#include "pool.h"

Pool::Pool() {}

void Pool::work() {
  job Job;
  while (true) {
    {
      std::unique_lock<std::mutex> lock(Queue_Mutex);
      condition.wait(lock,
                     [this]() { return !Queue.empty() || terminate_pool; });
      Job = Queue.front();
      Queue.pop();
    }
    // std::invoke(Job); // function<void()> type
    Job();
  }
}

void Pool::add_job(job j) {
  {
    std::unique_lock<std::mutex> lock{Queue_Mutex};
    Queue.push(j);
  }
  condition.notify_one();
}

void Pool::shutdown() {
  {
    std::unique_lock<std::mutex> lock{Queue_Mutex};
    terminate_pool = true;
  }
  condition.notify_all();
}
