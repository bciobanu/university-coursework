#ifndef _FIELD_RATIONAL_H_
#define _FIELD_RATIONAL_H_

#include <iostream>

#include "../groups/additive/rational.h"
#include "../groups/multiplicative/rational.h"
#include "../sets/rational_number.h"
#include "../utils/utils.h"

namespace crypto {

// Mediator
class RationalFieldElement :
    public utils::GenericGet<RationalNumber>,
    public AdditiveRational,
    public MultiplicativeRational {
  public:
    using utils::GenericGet<RationalNumber>::operator==;
    using utils::GenericGet<RationalNumber>::get;
    template<typename... Args> RationalFieldElement(Args&&...);
    friend std::ostream& operator <<(std::ostream&, const RationalFieldElement&);
};

}  // namespace crypto

#include "rational.cc"
#endif  // _FIELD_RATIONAL_H_