#ifndef _GROUPS_GENERIC_SET_ELEMENT_H_
#define _GROUPS_GENERIC_SET_ELEMENT_H_

#include <iostream>

namespace crypto {

template<typename T>
class GenericSetElement {
  public:
    GenericSetElement(T&);
    GenericSetElement(const GenericSetElement&) = default;
    GenericSetElement(GenericSetElement&&) = default;
    GenericSetElement& operator =(const GenericSetElement&) = default;
    GenericSetElement& operator =(GenericSetElement&&) = default;
    virtual ~GenericSetElement() = default;

  protected:
    T& el_;

  private:
    GenericSetElement() = delete;
};

}  // namespace crypto

#include "set_element.cc"
#endif  // _GROUPS_GENERIC_SET_ELEMENT_H_