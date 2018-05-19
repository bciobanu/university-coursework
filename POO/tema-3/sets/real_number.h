#ifndef _REAL_NUMBER_H_
#define _REAL_NUMBER_H_

#include <type_traits>

#include "../utils/utils.h"

namespace crypto {

template <typename T>
class RealNumber : public utils::GenericGet<T> {
    static_assert(std::is_floating_point_v<T>, "Expects floating point specialization");
  public:
    using utils::GenericGet<T>::GenericGet;
};

}  // namespace crypto

#endif  // _REAL_NUMBER_H_