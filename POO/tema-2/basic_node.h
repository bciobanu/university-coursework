#ifndef _BASIC_NODE_H
#define _BASIC_NODE_H

namespace avl {

template<typename T, typename U> 
class BasicNode {
 public:
  BasicNode(const U datum=U(), T* l=nullptr, T* r=nullptr);
  
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
