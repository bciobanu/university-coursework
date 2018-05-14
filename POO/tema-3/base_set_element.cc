#include "base_set_element.h"

namespace crypto {

template<typename T>
BaseSetElement<T>::BaseSetElement(const T el) : utils::GenericGet<T>(el) {}

template<typename T>
template<typename... Args>
BaseSetElement<T>::BaseSetElement(Args&&... args) : utils::GenericGet<T>(std::forward<Args>(args)...) {}

}  // namespace crpyto