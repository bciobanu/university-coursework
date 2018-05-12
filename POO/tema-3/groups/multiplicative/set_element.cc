#include "set_element.h"
#include "../set_element.h"

namespace crypto {

template<typename T>
MultiplicativeSetElement<T>::MultiplicativeSetElement(T& el) : GenericSetElement<T>(el) {}

template<typename T>
MultiplicativeSetElement<T>& MultiplicativeSetElement<T>::operator *=(const MultiplicativeSetElement& rhs) {
    this->el_.get() *= rhs.el_.get();
    return *this;
}

template<typename T>
MultiplicativeSetElement<T> MultiplicativeSetElement<T>::operator *(const MultiplicativeSetElement& rhs) const {
    return MultiplicativeSetElement<T>(*this) *= rhs;
}

}  // namespace crypto