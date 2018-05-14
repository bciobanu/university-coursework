#include "rational.h"

namespace crypto {

template<typename... Args>
RationalFieldElement::RationalFieldElement(Args&&... args) :
        utils::GenericGet<RationalNumber>(std::forward<Args>(args)...),
        AdditiveRational(get()),
        MultiplicativeRational(get()) {}

std::ostream& operator <<(std::ostream& os, const RationalFieldElement& rhs) {
    return os << rhs.get();
}

}  // namespace crypto