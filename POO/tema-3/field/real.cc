#include "real.h"

namespace crypto {

RealFieldElement::RealFieldElement(const double el) :
        AdditiveReal(el_), MultiplicativeReal(el_), el_(el) {}

bool RealFieldElement::operator ==(const RealFieldElement& rhs) {
    return el_ == rhs.el_;
}

std::ostream& operator <<(std::ostream& os, const RealFieldElement& rhs) {
    return os << rhs.el_;
}

}  // namespace crypto