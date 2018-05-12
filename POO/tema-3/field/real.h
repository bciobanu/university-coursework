#ifndef _FIELD_REAL_H_
#define _FIELD_REAL_H_

#include <iostream>

#include "../groups/additive/real.h"
#include "../groups/multiplicative/real.h"
#include "../sets/real_number.h"
#include "../utils/utils.h"

namespace crypto {

class RealFieldElement :
    public utils::GenericGet<RealNumber>,
    public AdditiveReal,
    public MultiplicativeReal {
  public:
    RealFieldElement(const double el=0);
};

}  // namespace crypto

#include "real.cc"
#endif  // _FIELD_REAL_H_