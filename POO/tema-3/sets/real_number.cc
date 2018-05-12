#include "real_number.h"
#include <stdexcept>

namespace crypto {

RealNumber::RealNumber(const double el) : BaseSetElement<double>(el) {
    if (not IsMember()) {
        throw std::invalid_argument("Invalid argument");
    }
}

bool RealNumber::IsMember() { return true; }

}  // namespace crypto