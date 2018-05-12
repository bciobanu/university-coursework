#ifndef _REAL_NUMBER_H_
#define _REAL_NUMBER_H_

#include "../base_set_element.h"

namespace crypto {

class RealNumber : public BaseSetElement<double> {
  public:
    RealNumber(const double el=0);
    bool IsMember() final;
    bool IsNull() final;
};

}  // namespace crypto

#include "real_number.cc"
#endif  // _REAL_NUMBER_H_