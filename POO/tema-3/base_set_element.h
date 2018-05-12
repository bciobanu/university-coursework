#ifndef _BASE_SET_ELEMENT_H_
#define _BASE_SET_ELEMENT_H_

#include <iostream>

namespace crypto {

template<typename T>
class BaseSetElement {
  public:
    BaseSetElement(const T el=T());
    BaseSetElement(const BaseSetElement&) = default;
    BaseSetElement& operator =(const BaseSetElement&) = default;
    BaseSetElement(BaseSetElement&&) = default;
    BaseSetElement& operator =(BaseSetElement&&) = default;
    virtual ~BaseSetElement() = default;

    T const& get() const;
    T& get();

    template<typename U>
    friend std::ostream& operator <<(std::ostream&, const BaseSetElement<U>&);

    virtual bool IsMember() = 0;

  private:
    T el_;
};

}  // namespace crypto

#include "base_set_element.cc"
#endif  // _BASE_SET_ELEMENT_H_