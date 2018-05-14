#ifndef _FIELD_REAL_H_
#define _FIELD_REAL_H_

#include <iostream>

#include "../groups/additive/real.h"
#include "../groups/multiplicative/real.h"
#include "../sets/real_number.h"
#include "../utils/utils.h"

namespace crypto {

// Mediator
class RealFieldElement :
    public utils::GenericGet<RealNumber>,
    public AdditiveReal,
    public MultiplicativeReal {
  public:
    using utils::GenericGet<RealNumber>::operator==;
    using utils::GenericGet<RealNumber>::get;
    RealFieldElement(const RealNumber el=0);
    friend std::ostream& operator <<(std::ostream&, const RealFieldElement&);
};

}  // namespace crypto

#include "real.cc"
#endif  // _FIELD_REAL_H_