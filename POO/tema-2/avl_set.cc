#include "avl_set.h"

namespace avl {

template<typename Key>
AvlSet<Key>::AvlSet() : root_(AvlNode<Key>::sentinel()) {}

template<typename Key>
AvlSet<Key>::~AvlSet() { Consume(root_); }

template<typename Key>
void AvlSet<Key>::Insert(const Key datum) { Add(root_, datum); }

template<typename Key>
std::ostream& operator <<(std::ostream& os, const AvlSet<Key>& rhs) { return os << *rhs.root_; }

template<typename Key>
void AvlSet<Key>::Consume(AvlNode<Key>*& node) {
  if (node == AvlNode<Key>::sentinel()) return;

  for (size_t dir : {0, 1}) {
    Consume(node->get(dir));
  }

  delete node;
  node = nullptr;
}

template<typename Key>
void AvlSet<Key>::Add(AvlNode<Key>*& node, const Key datum) {
  if (node == AvlNode<Key>::sentinel()) {
    node = new AvlNode<Key>(datum);
    return;
  }

  const size_t dir = !(datum < node->value());
  Add(node->get(dir), datum);
  if (node->get(dir)->height() > node->get(1 - dir)->height() + 1) {
    if (node->get(dir)->get(1 - dir)->height() > node->get(dir)->get(dir)->height()) {
      node->get(dir) = node->get(dir)->rotate(1 - dir);
    }

    node = node->rotate(dir);
  }

  node->refresh();
}

}  // namespace avl