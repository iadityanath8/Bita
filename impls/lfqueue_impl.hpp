#pragma once
#include "../include/lfqueue.hpp"

namespace Bita{
  template <class T>
    LfQueue<T>::LfQueue(){
      Node* dummy = new Node();
      head.store(dummy,std::memory_order_relaxed);
      tail.store(dummy,std::memory_order_relaxed);
    }

  template <class T>
    LfQueue<T>::~LfQueue(){
      while(Node* node = head.load()){
        head.store(node->next,std::memory_order_relaxed);
        delete node;
      }
    }

  template <class T>
    LfQueue<T>::LfQueue(LfQueue&& other)noexcept{
      head.store(other.head.load(std::memory_order_relaxed));
      tail.store(other.tail.load(std::memory_order_relaxed));
      other.head.store(nullptr,std::memory_order_relaxed);
      other.tail.store(nullptr,std::memory_order_relaxed);
    }

  template <class T>
    LfQueue<T>& LfQueue<T>::operator=(LfQueue&& other)noexcept{
      if(this != &other){
        while(Node* node = head.load()){
          head.store(node->next,std::memory_order_relaxed);
          delete node;
        }

        head.store(other.head.load(std::memory_order_relaxed));
        tail.store(other.head.load(std::memory_order_relaxed));
        other.head.store(nullptr,std::memory_order_relaxed);
        other.tail.store(nullptr,std::memory_order_relaxed);
      }
      return *this;
    }

  template <class T>
    void LfQueue<T>::enqueue(T value){
      std::atomic<Node*> new_node = new Node(value);
      Node* old_tail = tail.load();

      /*** CAS loop **/
      // while running or traversing and changing use compare and swap operation to change the node and guarantee that the node was accesses synchronously by multiple threads
      while(true){
        Node* next = old_tail->next.load();
        if(old_tail == tail.load()){
          if(next == nullptr){
            if(old_tail->next.compare_exchange_weak(next,new_node)){
              tail.compare_exchange_weak(old_tail,new_node);
              return;
            }
          }else{
            tail.compare_exchange_weak(old_tail,next);
          }
        }
      }  
      // CAS loop for making sure data changes syncgronously over all the threads in here
    }

  template <class T> 
    std::optional<T> LfQueue<T>::dequeue(){
      Node* old_head = head.load(); // load and store are pure atomic operation they do not need to explicit synchronize they are alreadty synchronized by atomic processor instruction 
      while(true){
        Node* tail_node = tail.load();
        Node* next = old_head->next.load();
        if(old_head == tail_node){
          if(next == nullptr){
            return std::nullopt;
          }
          tail.compare_exchange_weak(tail_node,next);
        }else{
          if(next != nullptr){
            auto t = next->m_data;
            if(!head.compare_exchange_weak(old_head,next)){
              return t;
            }
          }
        }
      }
    }

  template<class T>
    bool LfQueue<T>::empty(){
      Node* head_node = head.load();
      Node* next_node = head_node->next.load();
      return next_node == nullptr;
    }
}
