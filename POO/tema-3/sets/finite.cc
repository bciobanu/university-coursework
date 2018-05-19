#include <functional>  // std::function

#include "finite.h"

namespace crypto {

FiniteElement::IntMatrix FiniteElement::plus_, FiniteElement::times_;
FiniteElement::IntVector FiniteElement::inverse_plus_, FiniteElement::inverse_times_;
int FiniteElement::zero_;

void FiniteElement::Init(const IntMatrix& plus, const IntMatrix& times) {
    const int n = (int)plus.size();
    if (n <= 1) {
        throw std::logic_error("Invalid field order");
    }

    // n must be a prime power
    {
        int n_copy = n;
        for (int i = 2; i * i <= n_copy; ++i) {
            if (n_copy % i == 0) {
                do { n_copy /= i; } while (n_copy % i == 0);
                if (n_copy != 1) {
                    throw std::logic_error("Invalid field order");
                }
            }
        }
    }

    for (auto&& table : {plus, times}) {
        if ((int)table.size() != n) {
            throw std::logic_error("Wrong table size");
        }

        for (auto&& row : table) {
            if ((int)row.size() != n) {
                throw std::logic_error("Wrong table size");
            }
            for (auto el : row) {
                if (not (0 <= el and el < n)) {
                    throw std::out_of_range("Table values out of range");
                }
            }
        }
    }

    auto FindIdentity = [&n](const IntMatrix& op) -> int {
        int el = 0;
        while (el < n) {
            int a = 0;
            while (a < n and op[el][a] == a) {
                ++a;
            }
            if (a == n) {
                break;
            }
            ++el;
        }
        if (el == n) {
            throw std::logic_error("Missing identity element(s)");
        }
        return el;
    };

    // Find zero and one
    zero_ = FindIdentity(plus);
    int one_ = FindIdentity(times);

    for (auto&& table : {plus, times}) {
        for (int a = 0; a < n; ++a) {
            for (int b = 0; b < n; ++b) {
                if (table[a][b] != table[b][a]) {
                    throw std::logic_error("Missing commutativity");
                }

                for (int c = 0; c < n; ++c) {
                    if (table[table[a][b]][c] != table[a][table[b][c]]) {
                        throw std::logic_error("Missing associativity");
                    }
                }
            }
        }
    }

    // Distributivity of multiplication over addition
    for (int a = 0; a < n; ++a) {
        for (int b = 0; b < n; ++b) {
            for (int c = 0; c < n; ++c) {
                if (times[a][plus[b][c]] != plus[times[a][b]][times[a][c]]) {
                    throw std::logic_error("Missing distributivity");
                }
            }
        }
    }

    // Find addition inverses
    inverse_plus_.resize(n);
    for (int i = 0; i < n; ++i) {
        int j = 0;
        while (j < n and plus[i][j] != zero_) {
            ++j;
        }
        if (j == n) {
            throw std::logic_error("Missing addition inverse(s)");
        }
        inverse_plus_[i] = j;
    }

    inverse_times_.resize(n);
    for (int i = 0; i < n; ++i) {
        if (i == zero_) {
            inverse_times_[i] = -1;
            continue;
        }

        int j = 0;
        while (j < n and times[i][j] != one_) {
            ++j;
        }
        if (j == n) {
            throw std::logic_error("Missing multiplication inverse(s)");
        }
        inverse_times_[i] = j;
    }

    plus_ = plus;
    times_ = times;
}

FiniteElement::FiniteElement(int el) : el_(el) {
    if (el >= Cardinal()) {
        throw std::out_of_range("Element out of range");
    }
}

int FiniteElement::Cardinal() {
    return (int)plus_.size();
}

FiniteElement& FiniteElement::operator +=(const FiniteElement& rhs) {
    el_ = plus_[el_][rhs.el_];
    return *this;
}

FiniteElement& FiniteElement::operator *=(const FiniteElement& rhs) {
    el_ = times_[el_][rhs.el_];
    return *this;
}

FiniteElement& FiniteElement::operator -=(const FiniteElement& rhs) {
    el_ = plus_[el_][inverse_plus_[rhs.el_]];
    return *this;
}

FiniteElement& FiniteElement::operator /=(const FiniteElement& rhs) {
    if (rhs.el_ == zero_) {
        throw std::overflow_error("Division by zero");
    }
    el_ = times_[el_][inverse_times_[rhs.el_]];
    return *this;
}

bool FiniteElement::operator ==(const FiniteElement& rhs) {
    return el_ == rhs.el_;
}

std::ostream& operator <<(std::ostream& os, const FiniteElement& rhs) {
    return os << rhs.el_;
}

std::istream& operator >>(std::istream& is, FiniteElement& rhs) {
    int x; is >> x;
    rhs = FiniteElement(x);
    return is;
}

}  // namespace crypto

