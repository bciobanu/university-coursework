#ifndef _SETS_RATIONAL_NUMBER_H_
#define _SETS_RATIONAL_NUMBER_H_

#include <iostream>

#include "../utils/utils.h"
#include "../base_set_datatype.h"

namespace crypto {

// CRTP
template <typename T>
class Fraction : public BaseSetDataType<Fraction<T>> {
  public:
    Fraction(T=0, T=1);
    Fraction& operator *=(const Fraction&);
    Fraction& operator +=(const Fraction&);
    Fraction& operator /=(const Fraction&);
    Fraction& operator -=(const Fraction&);

    bool operator ==(const Fraction&) const;

    template <typename U>
    friend std::ostream& operator <<(std::ostream&, const Fraction<U>&);

  private:
    void Normalize();

    T numerator_, denominator_;
};

template <typename T>
class RationalNumber : public utils::GenericGet<Fraction<T>> {
    static_assert(std::is_integral<T>::value, "Expects integer specialization");
  public:
    using utils::GenericGet<Fraction<T>>::GenericGet;
};

}  // namespace crypto

#include "rational_number.cc"
#endif  // _SETS_RATIONAL_NUMBER_H_