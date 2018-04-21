#include "avl_set.h"

#include <algorithm>  // std::swap

namespace avl {

template<typename Key, typename Comp>
AvlSet<Key, Comp>::AvlSet() : root_(AvlNode<Key>::sentinel()), size_(0) {}

template<typename Key, typename Comp>
AvlSet<Key, Comp>::AvlSet(const AvlSet<Key, Comp>& oth) : root_(CopyStructure(oth.root_)), size_(oth.size_) {}

template<typename Key, typename Comp>
AvlSet<Key, Comp>::AvlSet(AvlSet&& oth) noexcept : root_(oth.root_), size_(oth.size_) {
  oth.root_ = AvlNode<Key>::sentinel();
  oth.size_ = 0;
}

template<typename Key, typename Comp>
AvlSet<Key, Comp>::~AvlSet() noexcept { Clear(); }

template<typename Key, typename Comp>
AvlSet<Key, Comp>& AvlSet<Key, Comp>::operator =(const AvlSet<Key, Comp>& oth) {
  auto tmp(oth);
  *this = std::move(tmp);
  return *this;
}

template<typename Key, typename Comp>
AvlSet<Key, Comp>& AvlSet<Key, Comp>::operator =(AvlSet<Key, Comp>&& oth) noexcept {
  if (this == &oth) return *this;
  
  Clear(root_);
  root_ = oth.root_;
  size_ = oth.size_;

  oth.root_ = AvlNode<Key>::sentinel();
  oth.size_ = 0;
  return *this;
}

template<typename Key, typename Comp>
void AvlSet<Key, Comp>::Insert(const Key datum) { Add(root_, datum); }

template<typename Key, typename Comp>
bool AvlSet<Key, Comp>::Find(const Key datum) const { return Search(root_, datum); }

template<typename Key, typename Comp>
template<typename... Args>
void AvlSet<Key, Comp>::Emplace(Args&&... args) { 
  Add(root_, Key(std::forward<Args>(args)...));
}

template<typename Key, typename Comp>
bool AvlSet<Key, Comp>::Empty() const { return size_ == 0; }

template<typename Key, typename Comp>
size_t AvlSet<Key, Comp>::Size() const { return size_; }

template<typename Key, typename Comp>
void AvlSet<Key, Comp>::Swap(AvlSet<Key, Comp>& oth) {
  std::swap(root_, oth.root_); 
  std::swap(size_, oth.size_);
}

template<typename Key, typename Comp>
void AvlSet<Key, Comp>::Clear() {
  Consume(root_);
  size_ = 0;
}

template<typename Key, typename Comp>
std::ostream& operator <<(std::ostream& os, const AvlSet<Key, Comp>& rhs) { 
  return os << *rhs.root_; 
}

template<typename Key, typename Comp>
void AvlSet<Key, Comp>::Consume(AvlNode<Key>*& node) {
  if (node == AvlNode<Key>::sentinel()) return;

  for (size_t dir : {0, 1}) {
    Consume(node->get(dir));
  }

  delete node;
  node = AvlNode<Key>::sentinel();
}

template<typename Key, typename Comp>
AvlNode<Key>* AvlSet<Key, Comp>::CopyStructure(const AvlNode<Key>* node) const {
  if (node == AvlNode<Key>::sentinel()) return AvlNode<Key>::sentinel();
  return new AvlNode<Key>(node->value(), 
                  CopyStructure(node->get(0)),
                  CopyStructure(node->get(1)));
}

template<typename Key, typename Comp>
void AvlSet<Key, Comp>::Add(AvlNode<Key>*& node, const Key datum) {
  if (node == AvlNode<Key>::sentinel()) {
    node = new AvlNode<Key>(datum);
    ++size_;
    return;
  }

  if (!Comp()(datum, node->value()) and !Comp()(node->value(), datum)) {  // equal 
    return;
  }

  const size_t dir = !Comp()(datum, node->value());
  Add(node->get(dir), datum);
  if (node->get(dir)->height() > node->get(1 - dir)->height() + 1) {
    if (node->get(dir)->get(1 - dir)->height() 
          > node->get(dir)->get(dir)->height()) {
      node->get(dir) = node->get(dir)->rotate(1 - dir);
    }

    node = node->rotate(dir);
  }

  node->refresh();
}

template<typename Key, typename Comp>
bool AvlSet<Key, Comp>::Search(const AvlNode<Key>* node, const Key datum) const {
  if (node == AvlNode<Key>::sentinel()) {
    return false;
  }
  if (!Comp()(datum, node->value()) and !Comp()(node->value(), datum)) {
    return true;
  }

  const size_t dir = !Comp()(datum, node->value());
  return Search(node->get(dir), datum);
}

}  // namespace avl