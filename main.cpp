#include "pool.h"
#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

using namespace std;

void amitabha(unsigned &co) {
  static std::atomic<unsigned> count{0};
  co = count.load();
  std::cout << "amitabha " << count++ << std::endl;
}

int main() {
  cout << "Hello World!" << endl;
  std::shared_ptr<std::mutex> mut = std::make_shared<std::mutex>();
  std::shared_ptr<std::condition_variable> con =
      std::make_shared<std::condition_variable>();
  unsigned co = 0;
  std::vector<thread> m_Threads;
  Pool pool{mut, con};
  auto Num_Threads = std::thread::hardware_concurrency();
  for (unsigned ii = 0; ii < Num_Threads; ii++) {
    m_Threads.push_back(std::thread(&Pool::work, &pool, std::ref(co)));
  }
  std::cout << "created " << Num_Threads << std::endl;
  for (size_t i = 0; i < 8; i++) {
    std::cout << "add job " << i << endl;
    pool.add_job(amitabha);
  }
  {
    std::unique_lock<std::mutex> lk{*mut};
    std::cout << "waiting in main for finish" << endl;
    con->wait(lk, [&co] { return co == 7; });
    std::cout << "co " << co << endl;
  }
  std::cout << "shutdown" << std::endl;
  pool.shutdown();
  std::cout << "join " << std::endl;
  for (size_t i = 0; i < m_Threads.size(); i++) {
    m_Threads.at(i).join();
  }
  return 0;
}
