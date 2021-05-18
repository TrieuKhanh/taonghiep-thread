#include "pool.h"
#include <iostream>
#include <thread>
#include <vector>

using namespace std;

void amitabha() {
  static unsigned count = 0;
  std::cout << "amitabha " << count << std::endl;
}

int main() {
  cout << "Hello World!" << endl;
  std::vector<thread> m_Threads;
  Pool pool;
  int Num_Threads = (int)std::thread::hardware_concurrency();
  for (auto ii = 0; ii < Num_Threads; ii++) {
    m_Threads.push_back(std::thread(&Pool::work, &pool));
  }
  std::cout << "created " << Num_Threads << std::endl;
  for (size_t i = 0; i < 50; i++) {
    pool.add_job(amitabha);
  }
  pool.shutdown();
  for (size_t i = 0; i < m_Threads.size(); i++) {
    m_Threads.at(i).join();
  }
  return 0;
}
