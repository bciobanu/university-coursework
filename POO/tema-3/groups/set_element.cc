#include "set_element.h"
#include "../base_set_element.h"
#include "../utils/utils.h"

namespace crypto {

template<typename T>
GenericSetElement<T>::GenericSetElement(T& el) : el_(el) {
    static_assert(utils::is_base_of_template<T, BaseSetElement>::value);
}


template<typename T>
std::ostream& operator <<(std::ostream& os, const GenericSetElement<T>& rhs) {
    return os << rhs.el_;
}

}  // namespace crypto