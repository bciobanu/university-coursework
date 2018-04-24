#ifndef _AVL_NODE_H
#define _AVL_NODE_H

#include "basic_node.h"
#include "sentinel_pool.h"

#include <iostream>

namespace avl {

template<typename U>
class AvlNode : public BasicNode<AvlNode<U>, U>, public SentinelPool<AvlNode<U>> {  // CRTP
 public:
  AvlNode();
  AvlNode(const U datum, 
    AvlNode<U>* l=SentinelPool<AvlNode<U>>::sentinel(), 
    AvlNode<U>* r=SentinelPool<AvlNode<U>>::sentinel());
  virtual ~AvlNode() = default;
  
  AvlNode(const AvlNode&) = delete;
  AvlNode(AvlNode&&) = delete;
  AvlNode& operator =(const AvlNode&) = delete;
  AvlNode& operator =(AvlNode&&) = delete;
  
  int height() const;
  void refresh();

  AvlNode* rotate(const size_t);
  
  template<typename T>
  friend std::ostream& operator <<(std::ostream& os, const AvlNode<T>& rhs);

 private: 
  int height_;
};

}  // namespace avl

#include "avl_node.cc"
#endif  // _AVL_NODE_H