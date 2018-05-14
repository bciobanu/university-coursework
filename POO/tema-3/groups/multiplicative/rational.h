#ifndef _GROUPS_MULTIPLICATIVE_RATIONAL_H_
#define _GROUPS_MULTIPLICATIVE_RATIONAL_H_

#include "../../sets/rational_number.h"
#include "set_element.h"

namespace crypto {

class MultiplicativeRational : public MultiplicativeSetElement<RationalNumber> {
  public:
    MultiplicativeRational(RationalNumber& el);
  private:
    MultiplicativeRational() = delete;
};

}  // namespace crypto

#include "rational.cc"
#endif  // _GROUPS_MULTIPLICATIVE_RATIONAL_H_