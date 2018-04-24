#ifndef _BASIC_NODE_H
#define _BASIC_NODE_H

#include <cstdint>

namespace avl {

template<typename T, typename U> 
class BasicNode {
 public:
  BasicNode(const U datum=U(), T* l=nullptr, T* r=nullptr);
  virtual ~BasicNode() = default;
  BasicNode(const BasicNode&) = delete;
  BasicNode(BasicNode&&) = delete;
  
  BasicNode& operator =(const BasicNode&) = delete;
  BasicNode& operator =(BasicNode&&) = delete;
  
  T* const& get(const size_t) const;
  T*& get(const size_t);

  const U& value() const;
  U& value();
 private:
  U datum_;
  T* son_[2];
};

}  // namespace avl

#include "basic_node.cc"
#endif  // _BASIC_NODE_H
