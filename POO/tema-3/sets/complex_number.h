#ifndef _COMPLEX_NUMBER_H_
#define _COMPLEX_NUMBER_H_

#include <complex>

#include "../utils/utils.h"

namespace crypto {

template <typename U>
class ComplexNumber : public utils::GenericGet<std::complex<U>> {
  public:
    using utils::GenericGet<std::complex<U>>::GenericGet;

    template <typename T>
    friend std::istream& operator >>(std::istream& is, ComplexNumber<T>& rhs) {
        T r(0), i(0);
        if (is.peek() == 'i') {
            is.get();
            is >> i;
        } else {
            is >> r;
        }
        rhs.get() = std::complex<T>(r, i);
        return is;
    }

};

}  // namespace crypto

#endif  // _COMPLEX_NUMBER_H_