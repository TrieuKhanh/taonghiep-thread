#ifndef POOL_H
#define POOL_H
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>

using job = std::function<void(unsigned &)>;

class Pool {
public:
  Pool(std::shared_ptr<std::mutex> m,
       std::shared_ptr<std::condition_variable> &con);
  void work(unsigned &co);
  void add_job(job j);
  void shutdown();

private:
  std::shared_ptr<std::condition_variable> condition;
  std::shared_ptr<std::mutex> Queue_Mutex;
  std::queue<job> Queue;
  bool terminate_pool{false};
};

#endif // POOL_H
