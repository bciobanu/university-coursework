#ifndef _GROUPS_MULTIPLICATIVE_SET_ELEMENT_H_
#define _GROUPS_MULTIPLICATIVE_SET_ELEMENT_H_

#include <iostream>

#include "../set_element.h"

namespace crypto {

template<typename T>
class MultiplicativeSetElement : public GenericSetElement<T> {
  public:
    MultiplicativeSetElement(T&);
    MultiplicativeSetElement<T>& operator *=(const MultiplicativeSetElement&);
    MultiplicativeSetElement<T> operator *(const MultiplicativeSetElement&) const;
};

}  // namespace crypto

#include "set_element.cc"
#endif  // _GROUPS_MULTIPLICATIVE_SET_ELEMENT_H_