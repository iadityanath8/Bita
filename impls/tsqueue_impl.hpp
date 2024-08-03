#pragma once
#include "../include/tsqueue.hpp"
#include <chrono>
#include <thread>

namespace Bita{
  template<class T>
    TsQueue<T>::TsQueue(std::initializer_list<T> init_list){
      std::lock_guard<std::mutex> lock(m_mtx);
      for(const auto& item : init_list){
        m_q.push(item);
      }
    }

  template<class T>
    TsQueue<T>::TsQueue(const TsQueue& other){
      std::lock_guard<std::mutex> lock(m_mtx);
      m_q = other.m_q;
    }

  template<class T>
    TsQueue<T>::TsQueue(TsQueue&& other){
      std::lock_guard<std::mutex> lock(m_mtx);
      m_q = std::move(other.m_q);
    }

  template<class T>
    TsQueue<T>& TsQueue<T>::operator =(const TsQueue& other)noexcept{
      if(this == &other) return *this;

      std::lock_guard<std::mutex> lock1(m_mtx);
      std::lock_guard<std::mutex> lock2(other.m_mtx);
      m_q = other.m_q;
      return *this;
    }

  template<class T> 
    TsQueue<T>& TsQueue<T>::operator=(TsQueue&& other)noexcept{
      if(this == &other) return *this;

      std::lock_guard<std::mutex> lock1(m_mtx);
      std::lock_guard<std::mutex> lock2(other.m_mtx);
      m_q = std::move(other.m_q);
      return *this;
    }

  template<class T>
    void TsQueue<T>::enqueue(T item){
      std::unique_lock<std::mutex> lock(m_mtx);
      m_q.push(item);
      m_cond.notify_one();  
    }

  template<class T>
    std::optional<T> TsQueue<T>::dequeue(){
      std::unique_lock<std::mutex> lock(m_mtx);
      if(m_cond.wait_for(lock,std::chrono::seconds(5),[this]() {return !m_q.empty();})){
        auto t = m_q.front();
        m_q.pop();
        return t;
      }
      return std::nullopt;
    }

  template<class T>
    bool TsQueue<T>::empty(){
      std::lock_guard<std::mutex> lock(m_mtx);
      return m_q.empty();
    }
}
