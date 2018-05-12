#include "set_element.h"

namespace crypto {

template<typename T>
MultiplicativeSetElement<T>::MultiplicativeSetElement(T& el) : utils::GenericGet<T&>(el) {}

template<typename T>
MultiplicativeSetElement<T>& MultiplicativeSetElement<T>::operator *=(const MultiplicativeSetElement& rhs) {
    this->get().get() *= rhs.get().get();
    return *this;
}

template<typename T>
MultiplicativeSetElement<T> MultiplicativeSetElement<T>::operator *(const MultiplicativeSetElement& rhs) const {
    return MultiplicativeSetElement<T>(*this) *= rhs;
}

template<typename T>
MultiplicativeSetElement<T>& MultiplicativeSetElement<T>::operator /=(const MultiplicativeSetElement& rhs) {
    if (rhs.get().IsNull()) {
        throw std::overflow_error("Division by zero");
    }
    this->get().get() /= rhs.get().get();
    return *this;
}

template<typename T>
MultiplicativeSetElement<T> MultiplicativeSetElement<T>::operator /(const MultiplicativeSetElement& rhs) const {
    return MultiplicativeSetElement<T>(*this) /= rhs;
}

}  // namespace crypto