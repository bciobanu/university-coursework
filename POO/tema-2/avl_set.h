#ifndef _AVL_SET_H
#define _AVL_SET_H

#include "avl_node.h"

namespace avl {

template<typename Key>
class AvlSet {
 public:
  AvlSet();
  virtual ~AvlSet();

  void Insert(const Key);

  template<typename T>
  friend std::ostream& operator <<(std::ostream& os, const AvlSet<T>& rhs);

 private:
  void Consume(AvlNode<Key>*&);
  
  void Add(AvlNode<Key>*&, const Key);

  AvlNode<Key>* root_; 
};

}  // namespace avl

#include "avl_set.cc"
#endif  // _AVL_SET_H