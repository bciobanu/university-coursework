#include <numeric>    // std::gcd
#include <stdexcept>  // std::logic_error
#include <algorithm>  // std::swap

#include "mod_number.h"

namespace crypto {

int ModInteger::mod_ = -1;

void ModInteger::SetModulo(int mod) {
    if (mod <= 0 || (mod > (1 << 30))) {
        throw std::out_of_range("Modulus out of range");
    }
    mod_ = mod;
}

ModInteger::ModInteger(int el) {
    if (mod_ == -1) {
        throw std::logic_error("Modulo not set");
    }
    el_ = el % mod_;
}

ModInteger& ModInteger::operator +=(const ModInteger& rhs) {
    el_ += rhs.el_;
    if (el_ >= mod_) {
        el_ -= mod_;
    }
    return *this;
}

ModInteger& ModInteger::operator -=(const ModInteger& rhs) {
    el_ -= rhs.el_;
    if (el_ < 0) {
        el_ += mod_;
    }
    return *this;
}

ModInteger& ModInteger::operator *=(const ModInteger& rhs) {
    el_ = ((long long)el_ * rhs.el_) % mod_;
    return *this;
}

ModInteger& ModInteger::operator /=(const ModInteger& rhs) {
    if (std::gcd(rhs.el_, mod_) != 1) {
        throw std::logic_error("rhs is not invertible");
    }

    return *this *= rhs.Inverse();
}

ModInteger ModInteger::Inverse() const {
    int x = el_, y = mod_;
    int u = 1, v = 0;
    while (y > 0) {
        const int d = x / y;
        x -= d * y; std::swap(x, y);
        u -= d * v; std::swap(u, v);
    }
    return ModInteger((mod_ + u) % mod_);
}

bool ModInteger::operator ==(const ModInteger& rhs) const {
    return el_ == rhs.el_;
}

std::ostream& operator <<(std::ostream& os, const ModInteger& rhs) {
    return os << rhs.el_;
}

std::istream& operator >>(std::istream& is, ModInteger& rhs) {
    while (is.peek() < '-') {
        is.get();
    }

    bool sign;
    if (is.peek() == '-') {
        sign = true;
        is.get();
    } else {
        sign = false;
    }

    rhs = ModInteger(0);
    do {
        rhs *= ModInteger(10);
        rhs += ModInteger(is.get() - '0');
    } while (is.peek() >= '0');
    return is;
}

}  // namespace crypto
