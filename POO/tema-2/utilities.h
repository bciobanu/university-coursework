#ifndef _UTILITIES_H
#define _UTILITIES_H

namespace avl {

template<typename T>
void Swap(T& a, T& b) noexcept {
  T aux = a;
  a = b;
  b = aux;
}

}  // namespace avl

#endif  // _UTILITIES_H