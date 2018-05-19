#ifndef _INTEGER_NUMBER_H_
#define _INTEGER_NUMBER_H_

#include <type_traits>

#include "../utils/utils.h"

namespace crypto {

template <typename T>
class IntegerNumber : public utils::GenericGet<T> {
    static_assert(std::is_integral_v<T>, "Expects integer data type");
  public:
    using utils::GenericGet<T>::GenericGet;
};

}  // namespace crypto

#endif  // _INTEGER_NUMBER_H_