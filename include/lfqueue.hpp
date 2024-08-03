#pragma once
#include <optional>
#include <atomic>

namespace Bita{
  template <class T>
    class LfQueue{
      public:
        /* constructor and destructor */
        LfQueue();
        ~LfQueue();

        /* copy constructor and copy assignment are deleted and cannot be used in this queue */
        LfQueue(const LfQueue&) = delete;
        LfQueue& operator=(const LfQueue&) = delete;

        /* move constructor and move assignment can be used */
        LfQueue(LfQueue&&) noexcept;
        LfQueue& operator=(LfQueue&&)noexcept;

        /* queue related operations */
        void enqueue(T value);
        std::optional<T> dequeue();
        bool empty();
      private:

        struct Node{
          T data;
          std::atomic<Node*> next;
          Node(T data_): data(data_), next(nullptr) {} 
          Node(){}
        };

        std::atomic<Node*> head;
        std::atomic<Node*> tail;
    };
}

