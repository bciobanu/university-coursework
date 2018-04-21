#ifndef _AVL_SET_H
#define _AVL_SET_H

#include "avl_node.h"

#include <functional>  // std::less

namespace avl {

template<typename Key, typename Comp=std::less<Key>>
class AvlSet {
 public:
  AvlSet();
  AvlSet(const AvlSet&);
  AvlSet(AvlSet&& oth) noexcept;
  virtual ~AvlSet() noexcept;
  AvlSet& operator =(const AvlSet&);
  AvlSet& operator =(AvlSet&&) noexcept;
  
  void Insert(const Key);
  bool Find(const Key) const;

  template<typename... Args>
  void Emplace(Args&&... args);

  void Erase(const Key);

  bool Empty() const;
  size_t Size() const;

  void Swap(AvlSet&);

  void Clear();

  template<typename K, typename C>
  friend std::ostream& operator <<(std::ostream& os, const AvlSet<K, C>& rhs);

 private:
  void Consume(AvlNode<Key>*&);
  AvlNode<Key>* CopyStructure(const AvlNode<Key>*) const;
  void Balance(AvlNode<Key>*&);

  void Add(AvlNode<Key>*&, const Key);
  bool Search(const AvlNode<Key>*, const Key) const;
  void Delete(AvlNode<Key>*&, const Key);
  Key DeleteMin(AvlNode<Key>*&) noexcept;
  
  AvlNode<Key>* root_; 
  size_t size_;
};

}  // namespace avl

#include "avl_set.cc"
#endif  // _AVL_SET_H