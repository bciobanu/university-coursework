#ifndef _SETS_RATIONAL_NUMBER_H_
#define _SETS_RATIONAL_NUMBER_H_

#include <iostream>

#include "../base_set_element.h"

namespace crypto {

class Fraction {
  using i64 = long long;
  public:
    Fraction(i64 numerator=0, i64 denominator=1);
    Fraction& operator *=(const Fraction&);
    Fraction& operator +=(const Fraction&);
    Fraction operator *(const Fraction&) const;
    Fraction operator +(const Fraction&) const;
    bool operator ==(const Fraction&) const;
    friend std::ostream& operator <<(std::ostream&, const Fraction&);
  private:
    void Normalize();
    i64 numerator_, denominator_;
};

class RationalNumber : public BaseSetElement<Fraction> {
  public:
    RationalNumber(const Fraction);

    template<typename... Args> RationalNumber(Args&&...);

    bool IsNull() final;
};

}  // namespace crypto

#include "rational_number.cc"
#endif  // _SETS_RATIONAL_NUMBER_H_