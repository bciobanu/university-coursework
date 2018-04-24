#include "basic_node.h"

#include <stdexcept> 

namespace avl {

template<typename T, typename U>
BasicNode<T, U>::BasicNode(const U datum, T* l, T* r) : datum_(datum), son_{l, r} {}

template<typename T, typename U>
T* const& BasicNode<T, U>::get(const size_t dir) const {
  if (dir > 1) {  // 2x slower
    throw std::out_of_range("BasicNode<T, U>::get() : dir is out of range");
  }

  return son_[dir]; 
}

template<typename T, typename U>
T*& BasicNode<T, U>::get(const size_t dir) { 
  return const_cast<T*&>(static_cast<const BasicNode<T, U>&>(*this).get(dir)); 
}

template<typename T, typename U>
const U& BasicNode<T, U>::value() const { return datum_; }

template<typename T, typename U>
U& BasicNode<T, U>::value() {
  return const_cast<U&>(static_cast<const BasicNode<T, U>&>(*this).value());
}

}  // namespace avl