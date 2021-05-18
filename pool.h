#ifndef POOL_H
#define POOL_H
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>

using job = std::function<void(void)>;

class Pool {
public:
  Pool();
  void work();
  void add_job(job j);
  void shutdown();

private:
  std::condition_variable condition;
  std::mutex Queue_Mutex;
  std::queue<job> Queue;
  bool terminate_pool;
};

#endif // POOL_H
