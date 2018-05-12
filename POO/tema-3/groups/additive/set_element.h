#ifndef _GROUPS_ADDITIVE_SET_ELEMENT_H_
#define _GROUPS_ADDITIVE_SET_ELEMENT_H_

#include <iostream>

#include "../set_element.h"

namespace crypto {

template<typename T>
class AdditiveSetElement : public GenericSetElement<T> {
  public:
    AdditiveSetElement(T&);
    AdditiveSetElement<T>& operator +=(const AdditiveSetElement&);
    AdditiveSetElement<T> operator +(const AdditiveSetElement&) const;
};

}  // namespace crypto

#include "set_element.cc"
#endif  // _GROUPS_ADDITIVE_SET_ELEMENT_H_