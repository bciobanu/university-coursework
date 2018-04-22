#include "sentinel_pool.h"

namespace avl {

template<typename T> int SentinelPool<T>::objects_alive_(0);
template<typename T> T* SentinelPool<T>::sentinel_(nullptr);

template<typename T>
SentinelPool<T>::SentinelPool() {
  ++objects_alive_;
}

template<typename T>
SentinelPool<T>::~SentinelPool() {
  --objects_alive_;
  if (objects_alive_ == 1) {  // the sentinel also counts
    delete sentinel_; 
    sentinel_ = nullptr;
  }
}

template<typename T>
T* const& SentinelPool<T>::sentinel() {
  if (sentinel_ == nullptr) {
    sentinel_ = new T();
  }
  return sentinel_;
}

}  // namespace avl