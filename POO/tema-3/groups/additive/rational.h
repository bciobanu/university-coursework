#ifndef _GROUPS_ADDITIVE_RATIONAL_H_
#define _GROUPS_ADDITIVE_RATIONAL_H_

#include "../../sets/rational_number.h"
#include "set_element.h"

namespace crypto {

class AdditiveRational : public AdditiveSetElement<RationalNumber> {
  public:
    AdditiveRational(RationalNumber& el);
  private:
    AdditiveRational() = delete;
};

}  // namespace crypto

#include "rational.cc"
#endif  // _GROUPS_ADDITIVE_RATIONAL_H_