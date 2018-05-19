#ifndef _UTILS_H_
#define _UTILS_H_

#include <type_traits>
#include <utility>
#include <iostream>

#include "../base_set_datatype.h"

namespace utils {

template <template <typename...> class Base, typename... Ts>
std::true_type IsBaseOfImpl(const Base<Ts...>*);

template <template <typename...> class Base>
std::false_type IsBaseOfImpl(...);

template <template <typename...> class Base, typename T>
using IsBaseOf = decltype(IsBaseOfImpl<Base>(std::declval<T*>()));

template <typename T, typename U>
constexpr inline bool IsSelf() {
    return std::is_same<U, std::decay_t<T>>::value
     or std::is_base_of<U, std::decay_t<T>>::value;
}

template <typename T>
class GenericGet : public crypto::BaseSetDataType<GenericGet<T>> {
  public:
    GenericGet(const T el=T()) : el_(el) {}

    template <typename... Args>
    GenericGet(Args&&... args) : el_(std::forward<Args>(args)...) {}

    GenericGet(const GenericGet&) = default;
    GenericGet(GenericGet&&) = default;
    GenericGet& operator =(const GenericGet&) = default;
    GenericGet& operator =(GenericGet&&) = default;
    virtual ~GenericGet() = default;

    T const& get() const { return el_; }
    T& get() { return el_; }

    GenericGet& operator +=(const GenericGet& rhs) { el_ += rhs.el_; return *this; }
    GenericGet& operator -=(const GenericGet& rhs) { el_ -= rhs.el_; return *this; }
    GenericGet& operator *=(const GenericGet& rhs) { el_ *= rhs.el_; return *this; }
    GenericGet& operator /=(const GenericGet& rhs) { el_ /= rhs.el_; return *this; }

    template <typename U>
    friend std::ostream& operator <<(std::ostream& os, const GenericGet<U>& rhs) { return os << rhs.get(); }

    bool operator ==(const GenericGet& rhs) { return el_ == rhs.el_; }

  private:
    T el_;
};

}  // namespace utils

#endif  // _UTILS_H_
