#ifndef _AVL_SET_H
#define _AVL_SET_H

#include "avl_node.h"
#include "set_interface.h"

#include <functional>  // std::less

namespace avl {

template<typename Key, typename Comp=std::less<Key>>
class AvlSet : public SetInterface<Key, Comp> {
 public:
  AvlSet();
  AvlSet(const AvlSet&);
  AvlSet(const Key*, const Key*);
  AvlSet(AvlSet&& oth) noexcept;
  virtual ~AvlSet() noexcept;
  AvlSet& operator =(const AvlSet&);
  AvlSet& operator =(AvlSet&&) noexcept;
  
  void Insert(const Key) override;
  bool Find(const Key) const override;
  void Erase(const Key) override;

  bool Empty() const override;
  size_t Size() const override;

  void Clear() override;

  template<typename... Args>
  void Emplace(Args&&... args);
  
  void Swap(AvlSet&);

  AvlSet& operator +=(AvlSet);
  AvlSet operator +(const AvlSet&) const;

  template<typename K, typename C>
  friend std::istream& operator >>(std::istream&, AvlSet<K, C>&);

  template<typename K, typename C>
  friend std::ostream& operator <<(std::ostream&, const AvlSet<K, C>&);

 private:
  void Consume(AvlNode<Key>*&);
  AvlNode<Key>* CopyStructure(const AvlNode<Key>*) const;

  AvlNode<Key>* ArrayBuild(const Key*, const Key*) const;
  AvlNode<Key>* BuildSet(const Key*, const Key*) const;
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