#include <numeric>    // std::gcd
#include <stdexcept>  // std::overflow_error

#include "rational_number.h"

namespace crypto {

template <typename T>
Fraction<T>::Fraction(T numerator, T denominator) :
        numerator_(numerator), denominator_(denominator) {
    Normalize();
}

template <typename T>
Fraction<T>& Fraction<T>::operator *=(const Fraction& rhs) {
    numerator_ *= rhs.numerator_;
    denominator_ *= rhs.denominator_;
    Normalize();
    return *this;
}

template <typename T>
Fraction<T>& Fraction<T>::operator +=(const Fraction& rhs) {
    numerator_ = numerator_ * rhs.denominator_ + denominator_ * rhs.numerator_;
    denominator_ *= rhs.denominator_;
    Normalize();
    return *this;
}

template <typename T>
Fraction<T>& Fraction<T>::operator /=(const Fraction& rhs) {
    if (rhs == static_cast<Fraction>(0)) {
        throw std::overflow_error("Division by zero");
    }
    return *this *= Fraction(rhs.denominator_, rhs.numerator_);
}

template <typename T>
Fraction<T>& Fraction<T>::operator -=(const Fraction& rhs) {
    return *this += Fraction(-rhs.numerator_, rhs.denominator_);
}

template <typename T>
bool Fraction<T>::operator ==(const Fraction& rhs) const {
    return numerator_ == rhs.numerator_ and denominator_ == rhs.denominator_;
}

template <typename T>
std::ostream& operator <<(std::ostream& os, const Fraction<T>& rhs) {
    return os << rhs.numerator_ << '/' << rhs.denominator_;
}

template <typename T>
void Fraction<T>::Normalize() {
    if (denominator_ < static_cast<T>(0)) {
        numerator_ = -numerator_;
        denominator_ = -denominator_;
    }

    T g = std::gcd(numerator_, denominator_);
    numerator_ /= g;
    denominator_ /= g;
}

}  // namespace crypto
