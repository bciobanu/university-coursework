#include <numeric>  // std::gcd

#include "rational_number.h"

namespace crypto {

Fraction::Fraction(i64 numerator, i64 denominator) :
        numerator_(numerator), denominator_(denominator) {
    Normalize();
}

Fraction& Fraction::operator *=(const Fraction& rhs) {
    numerator_ *= rhs.numerator_;
    denominator_ *= rhs.denominator_;
    Normalize();
    return *this;
}

Fraction& Fraction::operator +=(const Fraction& rhs) {
    numerator_ = numerator_ * rhs.denominator_ + denominator_ * rhs.numerator_;
    denominator_ *= rhs.denominator_;
    Normalize();
    return *this;
}

Fraction& Fraction::operator /=(const Fraction& rhs) {
    return *this *= Fraction(rhs.denominator_, rhs.numerator_);
}

Fraction& Fraction::operator -=(const Fraction& rhs) {
    return *this += Fraction(-rhs.numerator_, rhs.denominator_);
}

bool Fraction::operator ==(const Fraction& rhs) const {
    return numerator_ == rhs.numerator_ and denominator_ == rhs.denominator_;
}

std::ostream& operator <<(std::ostream& os, const Fraction& rhs) {
    return os << rhs.numerator_ << '/' << rhs.denominator_;
}

void Fraction::Normalize() {
    if (denominator_ < 0LL) {
        numerator_ = -numerator_;
        denominator_ = -denominator_;
    }

    i64 g = std::gcd(numerator_, denominator_);
    numerator_ /= g;
    denominator_ /= g;
}

RationalNumber::RationalNumber(const Fraction el) : BaseSetElement<Fraction>(el) {}

template<typename... Args>
RationalNumber::RationalNumber(Args&&... args) : BaseSetElement<Fraction>(std::forward<Args>(args)...) {}

bool RationalNumber::IsNull() {
    return get() == Fraction(0);
}

}  // namespace crypto
