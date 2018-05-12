#ifndef _GROUPS_ADDITIVE_REAL_H_
#define _GROUPS_ADDITIVE_REAL_H_

#include "../../sets/real_number.h"
#include "set_element.h"

namespace crypto {

class AdditiveReal : public AdditiveSetElement<RealNumber> {
  public:
    AdditiveReal(RealNumber& el);
  private:
    AdditiveReal() = delete;
};

}  // namespace crypto

#include "real.cc"
#endif  // _GROUPS_ADDITIVE_REAL_H_