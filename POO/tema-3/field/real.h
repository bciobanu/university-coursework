#ifndef _FIELD_REAL_H_
#define _FIELD_REAL_H_

#include <iostream>

#include "../groups/additive/real.h"
#include "../groups/multiplicative/real.h"
#include "../sets/real_number.h"

namespace crypto {

class RealFieldElement : public AdditiveReal, public MultiplicativeReal {
  public:
    RealFieldElement(const double el=0);

    bool operator ==(const RealFieldElement&);

    friend std::ostream& operator <<(std::ostream&, const RealFieldElement&);

  private:
    RealNumber el_;
};

}  // namespace crypto

#include "real.cc"
#endif  // _FIELD_REAL_H_