#ifndef _GROUPS_MULTIPLICATIVE_REAL_H_
#define _GROUPS_MULTIPLICATIVE_REAL_H_

#include "../../sets/real_number.h"
#include "set_element.h"

namespace crypto {

class MultiplicativeReal : public MultiplicativeSetElement<RealNumber> {
  public:
    MultiplicativeReal(RealNumber& el);
  private:
    MultiplicativeReal() = delete;
};

}  // namespace crypto

#include "real.cc"
#endif  // _GROUPS_MULTIPLICATIVE_REAL_H_