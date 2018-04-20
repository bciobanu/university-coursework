#include "basic_node.h"

namespace avl {

template<typename T, typename U>
BasicNode<T, U>::BasicNode(const U datum, T* l, T* r) : datum_(datum), son_{l, r} {}

template<typename T, typename U>
T* const& BasicNode<T, U>::get(const size_t dir) const {  // TODO: exceptions
  return son_[dir]; 
}

template<typename T, typename U>
T*& BasicNode<T, U>::get(const size_t dir) { 
  return const_cast<T*&>(static_cast<const BasicNode<T, U>&>(*this).get(dir)); 
}

template<typename T, typename U>
U BasicNode<T, U>::value() const { return datum_; }

}  // namespace avl