#include <numeric>    // std::gcd
#include <stdexcept>  // std::logic_error
#include <algorithm>  // std::swap

#include "mod_number.h"

namespace crypto {

template<int mod>
ModInteger<mod>::ModInteger(const int el) : el_(el % mod) {}

template<int mod>
ModInteger<mod>& ModInteger<mod>::operator +=(const ModInteger& rhs) {
    el_ += rhs.el_;
    if (el_ >= mod) {
        el_ -= mod;
    }
    return *this;
}

template<int mod>
ModInteger<mod>& ModInteger<mod>::operator -=(const ModInteger& rhs) {
    el_ -= rhs.el_;
    if (el_ < 0) {
        el_ += mod;
    }
    return *this;
}

template<int mod>
ModInteger<mod>& ModInteger<mod>::operator *=(const ModInteger& rhs) {
    el_ = ((long long)el_ * rhs.el_) % mod;
    return *this;
}

template<int mod>
ModInteger<mod>& ModInteger<mod>::operator /=(const ModInteger& rhs) {
    if (std::gcd(rhs.el_, mod) != 1) {
        throw std::logic_error("rhs is not invertible");
    }
    return *this *= rhs.Inverse();
}

template<int mod>
ModInteger<mod> ModInteger<mod>::Inverse() const {
    int x = el_, y = mod;
    int u = 1, v = 0;
    while (y > 0) {
        const int d = x / y;
        x -= d * y; std::swap(x, y);
        u -= d * v; std::swap(u, v);
    }
    return ModInteger((mod + u) % mod);
}

template<int mod>
bool ModInteger<mod>::operator ==(const ModInteger& rhs) const {
    return el_ == rhs.el_;
}

template<int mod>
std::ostream& operator <<(std::ostream& os, const ModInteger<mod>& rhs) {
    return os << rhs.el_;
}

}  // namespace crypto
