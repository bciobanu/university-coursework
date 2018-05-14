#include "real.h"

namespace crypto {

RealFieldElement::RealFieldElement(const RealNumber el) :
        utils::GenericGet<RealNumber>(el),
        AdditiveSetElement<RealNumber>(get()),
        MultiplicativeSetElement<RealNumber>(get()) {}

std::ostream& operator <<(std::ostream& os, const RealFieldElement& rhs) {
    return os << rhs.get();
}

}  // namespace crypto