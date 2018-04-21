#ifndef _AVL_NODE_H
#define _AVL_NODE_H

#include "basic_node.h"
#include <iostream>

namespace avl {

template<typename U>
class AvlNode : public BasicNode<AvlNode<U>, U> {  // CRTP
 public:
  AvlNode();
  AvlNode(const U datum, AvlNode* l=sentinel(), AvlNode* r=sentinel());
  AvlNode(const AvlNode&) = delete;
  AvlNode& operator =(const AvlNode&) = delete;
  
  int height() const;
  void refresh();

  AvlNode* rotate(const size_t);
  
  template<typename T>
  friend std::ostream& operator <<(std::ostream& os, const AvlNode<T>& rhs);

  static AvlNode* const& sentinel();

 private: 
  int height_;
};

}  // namespace avl

#include "avl_node.cc"
#endif  // _AVL_NODE_H