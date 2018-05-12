#include "real.h"

namespace crypto {

RealFieldElement::RealFieldElement(const double el) :
        utils::GenericGet<RealNumber>(el), AdditiveReal(this->get()), MultiplicativeReal(this->get()) {}

}  // namespace crypto