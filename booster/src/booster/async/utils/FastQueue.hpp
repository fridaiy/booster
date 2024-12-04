#ifndef booster_async_utils_FastQueue_hpp
#define booster_async_utils_FastQueue_hpp

#include "booster/Environment.hpp"

namespace booster { namespace async { namespace utils {
  
template<typename T>
class FastQueue {
public:
  
  FastQueue()
    : first(nullptr)
    , last(nullptr)
    , count(0)
  {}
  
  ~FastQueue(){
    clear();
  }

  FastQueue(const FastQueue &) = delete;

  FastQueue(FastQueue &&other) noexcept
    : FastQueue() {
      using std::swap;
      swap(first, other.first);
      swap(last, other.last);
      swap(count, other.count);
  }

  FastQueue &operator=(const FastQueue &) = delete;

  FastQueue &operator=(FastQueue &&other) noexcept {
      if (this != std::addressof(other)) {
          using std::swap;
          swap(first, other.first);
          swap(last, other.last);
          swap(count, other.count);
      }
      return *this;
  }

  
  T* first;
  T* last;
  v_int32 count{};

  v_int32 Count() {
      return count;
  }

  bool empty() {
      return count == 0;
  }
  
  void pushFront(T* entry) {
    entry->_ref = first;
    first = entry;
    if(last == nullptr) {
      last = first;
    }
    ++ count;
  }
  
  void pushBack(T* entry) {
    entry->_ref = nullptr;
    if(last == nullptr) {
      first = entry;
      last = entry;
    } else {
      last->_ref = entry;
      last = entry;
    }
    ++ count;
  }
  
  void round(){
    if(count > 1) {
      last->_ref = first;
      last = first;
      first = first->_ref;
      last->_ref = nullptr;
    }
  }
  
  T* popFront() {
    T* result = first;
    first = first->_ref;
    if(first == nullptr) {
      last = nullptr;
    }
    -- count;
    return result;
  }
  
  void popFrontNoData() {
    T* result = first;
    first = first->_ref;
    if(first == nullptr) {
      last = nullptr;
    }
    delete result;
    -- count;
  }

  static void moveAll(FastQueue& fromQueue, FastQueue& toQueue) {

    if(fromQueue.count > 0) {

      if (toQueue.last == nullptr) {
        toQueue.first = fromQueue.first;
        toQueue.last = fromQueue.last;
      } else {
        toQueue.last->_ref = fromQueue.first;
        toQueue.last = fromQueue.last;
      }

      toQueue.count += fromQueue.count;
      fromQueue.count = 0;

      fromQueue.first = nullptr;
      fromQueue.last = nullptr;

    }

  }

  void cutEntry(T* entry, T* prevEntry){

    if(prevEntry == nullptr) {
      popFront();
    } else {
      prevEntry->_ref = entry->_ref;
      -- count;
      if(prevEntry->_ref == nullptr) {
        last = prevEntry;
      }
    }

  }
  
  void clear() {
    T* curr = first;
    while (curr != nullptr) {
      T* next = curr->_ref;
      delete curr;
      curr = next;
    }
    first = nullptr;
    last = nullptr;
    count = 0;
  }
  
};
  
}}}

#endif
