#ifndef _COMPLEX_NUMBER_H_
#define _COMPLEX_NUMBER_H_

#include <complex>

#include "../utils/utils.h"

namespace crypto {

template <typename T>
std::istream& operator >>(std::istream& is, std::complex<T>& rhs) {
    T aux; is >> aux;
    rhs.real() = aux;
    const std::streampos from = is.tellg();
    is >> aux;
    if (is.peek() != 'i') {
        is.seekg(from);
    } else {
        is.get();
        rhs.imag() = aux;
    }
    return is;
}

template <typename T>
class ComplexNumber : public utils::GenericGet<std::complex<T>> {
  public:
    using utils::GenericGet<std::complex<T>>::GenericGet;
};

}  // namespace crypto

#endif  // _COMPLEX_NUMBER_H_