#ifndef _SENTINEL_POOL_H
#define _SENTINEL_POOL_H

namespace avl {

template<typename T>
class SentinelPool {
 public:
  SentinelPool();
  virtual ~SentinelPool();
  static T* const& sentinel();
 private:
  static int objects_alive_;
  static T* sentinel_; 
};

}  // namespace avl

#include "sentinel_pool.cc"
#endif  // _SENTINEL_POOL_H