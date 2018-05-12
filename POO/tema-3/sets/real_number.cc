#include "real_number.h"
#include <stdexcept>
#include <cmath>

namespace crypto {

RealNumber::RealNumber(const double el) : BaseSetElement<double>(el) {
    if (not IsMember()) {
        throw std::invalid_argument("Invalid argument");
    }
}

bool RealNumber::IsMember() { return true; }

bool RealNumber::IsNull() { return fabs(get()) < 1e-6; }

}  // namespace crypto