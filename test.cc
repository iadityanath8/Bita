#include "bita.hpp"

#include <iostream>
#include <thread>
#include <chrono>

Bita::TsQueue<int> q;
Bita::LfQueue<int> q1;

void producer(){
  for(int i = 0;i < 10;i++){
    std::cout << "Sent" << i << std::endl;
    q.enqueue(i);
  }
}

void consumer(){
  while(true){
    auto a = q.dequeue();
    std::cout << "received" << a.value() << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
}

int main(){
  std::thread th[2];
  for(int i = 0;i < 2;i++){
    th[i] = std::thread(producer);
  }

  consumer();
  for(int i = 0;i < 2;i++){
    th[i].join();
  }
  return 0;
}
