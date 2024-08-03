#pragma once

#include <condition_variable>
#include <mutex>
#include <optional>
#include <queue>

namespace Bita{
  template<class T>
    class TsQueue{
      public:

        // constructor
        TsQueue() = default;
        TsQueue(std::initializer_list<T>);

        // copy consrtructors and move constructors
        TsQueue(const TsQueue&);
        TsQueue(TsQueue&&);

        // assignment copy and move 
        TsQueue& operator=(const TsQueue&)noexcept;
        TsQueue& operator=(TsQueue&&)noexcept;

        // queue related thread safe function 
        void enqueue(T);
        std::optional<T> dequeue();

        bool empty();
      private:

        std::queue<T> m_q;  // soon we will replace it with ring buffer ir our custome data structure 
        std::mutex m_mtx;
        std::condition_variable m_cond;
    };
}

