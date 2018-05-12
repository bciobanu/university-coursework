#include "base_set_element.h"

namespace crypto {

template<typename T>
BaseSetElement<T>::BaseSetElement(const T el) : el_(el) {}

template<typename T>
T const& BaseSetElement<T>::get() const { return el_; }

template<typename T>
T& BaseSetElement<T>::get() {
    return const_cast<T&>(static_cast<const BaseSetElement<T>&>(*this).get());
}

template<typename T>
std::ostream& operator <<(std::ostream& os, const BaseSetElement<T>& rhs) {
    return os << rhs.el_;
}

}  // namespace crpyto