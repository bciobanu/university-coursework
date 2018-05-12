#include "set_element.h"

namespace crypto {

template<typename T>
AdditiveSetElement<T>::AdditiveSetElement(T& el) : utils::GenericGet<T&>(el) {}

template<typename T>
AdditiveSetElement<T>& AdditiveSetElement<T>::operator +=(const AdditiveSetElement& rhs) {
    this->get().get() += rhs.get().get();
    return *this;
}

template<typename T>
AdditiveSetElement<T> AdditiveSetElement<T>::operator +(const AdditiveSetElement& rhs) const {
    return AdditiveSetElement<T>(*this) += rhs;
}

template<typename T>
AdditiveSetElement<T>& AdditiveSetElement<T>::operator -=(const AdditiveSetElement& rhs) {
    this->get().get() -= rhs.get().get();
    return *this;
}

template<typename T>
AdditiveSetElement<T> AdditiveSetElement<T>::operator -(const AdditiveSetElement& rhs) const {
    return AdditiveSetElement<T>(*this) -= rhs;
}

}  // namespace crypto