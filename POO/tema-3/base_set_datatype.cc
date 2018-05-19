#include "base_set_datatype.h"

namespace cryto {

template <typename DerivedDataType>
DerivedDataType operator +(
        const crypto::BaseSetDataType<DerivedDataType>& lhs,
        const crypto::BaseSetDataType<DerivedDataType>& rhs) {
    return DerivedDataType(lhs) += rhs;
}

template <typename DerivedDataType>
DerivedDataType operator -(
        const crypto::BaseSetDataType<DerivedDataType>& lhs,
        const crypto::BaseSetDataType<DerivedDataType>& rhs) {
    return DerivedDataType(lhs) -= rhs;
}

template <typename DerivedDataType>
DerivedDataType operator *(
        const crypto::BaseSetDataType<DerivedDataType>& lhs,
        const crypto::BaseSetDataType<DerivedDataType>& rhs) {
    return DerivedDataType(lhs) *= rhs;
}

template <typename DerivedDataType>
DerivedDataType operator /(
        const crypto::BaseSetDataType<DerivedDataType>& lhs,
        const crypto::BaseSetDataType<DerivedDataType>& rhs) {
    return DerivedDataType(lhs) /= rhs;
}

}  // namespace crypo
