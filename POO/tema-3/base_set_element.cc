#include "base_set_element.h"

namespace crypto {

template<typename T>
BaseSetElement<T>::BaseSetElement(const T el) : utils::GenericGet<T>(el) {}

}  // namespace crpyto