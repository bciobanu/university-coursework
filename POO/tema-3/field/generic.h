#ifndef _FIELD_GENERIC_H_
#define _FIELD_GENERIC_H_

#include <type_traits>

#include "../base_set_datatype.h"
#include "../utils/utils.h"

namespace crypto {

template <typename T>
class GenericFieldElement : public utils::GenericGet<T> {
    static_assert(utils::IsBaseOf<BaseSetDataType, T>::value,
        "Expects field arithmetic");
  public:
    using utils::GenericGet<T>::GenericGet;
    template <
        typename Head, typename... Tail,
        typename = std::enable_if_t <
            not utils::IsSelf<Head, GenericFieldElement<T>>()
        >
    >
    GenericFieldElement(const Head& head, Tail&&... tail)
        : utils::GenericGet<T>(head, std::forward<Tail>(tail)...) {}
};

}  // namespace crypto

#endif  // _FIELD_GENERIC_H_