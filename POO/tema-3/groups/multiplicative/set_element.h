#ifndef _GROUPS_MULTIPLICATIVE_SET_ELEMENT_H_
#define _GROUPS_MULTIPLICATIVE_SET_ELEMENT_H_

#include "../../utils/utils.h"

namespace crypto {

template<typename T>
class MultiplicativeSetElement : public utils::GenericGet<T&> {
  public:
    MultiplicativeSetElement(T&);
    MultiplicativeSetElement<T>& operator *=(const MultiplicativeSetElement&);
    MultiplicativeSetElement<T> operator *(const MultiplicativeSetElement&) const;
    MultiplicativeSetElement<T>& operator /=(const MultiplicativeSetElement&);
    MultiplicativeSetElement<T> operator /(const MultiplicativeSetElement&) const;
};

}  // namespace crypto

#include "set_element.cc"
#endif  // _GROUPS_MULTIPLICATIVE_SET_ELEMENT_H_