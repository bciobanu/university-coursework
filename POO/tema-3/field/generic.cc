#include "generic.h"

namespace crypto {

template <typename T>
template <typename Head, typename... Tail, typename>
GenericFieldElement<T, std::enable_if_t<utils::IsBaseOf<BaseSetDataType, T>::value>>
        ::GenericFieldElement(Head&& h, Tail&&... tail)
    : utils::GenericGet<T>(std::forward<Head>(h), std::forward<Tail>(tail)...) {}

}  // namespace crypto
