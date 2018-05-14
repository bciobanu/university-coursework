#include "real.h"

namespace crypto {

RealFieldElement::RealFieldElement(const RealNumber el) :
        utils::GenericGet<RealNumber>(el),
        AdditiveReal(get()),
        MultiplicativeReal(get()) {}

std::ostream& operator <<(std::ostream& os, const RealFieldElement& rhs) {
    return os << rhs.get();
}

}  // namespace crypto