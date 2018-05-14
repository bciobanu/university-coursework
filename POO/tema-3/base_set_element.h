#ifndef _BASE_SET_ELEMENT_H_
#define _BASE_SET_ELEMENT_H_

#include <iostream>

#include "utils/utils.h"

namespace crypto {

template<typename T>
class BaseSetElement : public utils::GenericGet<T> {
  public:
    BaseSetElement(const T el=T());

    template<typename... Args> BaseSetElement(Args&&...);

    virtual bool IsNull() = 0;
};

}  // namespace crypto

#include "base_set_element.cc"
#endif  // _BASE_SET_ELEMENT_H_