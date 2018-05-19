#ifndef _COMPLEX_NUMBER_H_
#define _COMPLEX_NUMBER_H_

#include <complex>

#include "../utils/utils.h"

namespace crypto {

template <typename T>
class ComplexNumber : public utils::GenericGet<std::complex<T>> {
  public:
    using utils::GenericGet<std::complex<T>>::GenericGet;
};

}  // namespace crypto

#endif  // _COMPLEX_NUMBER_H_