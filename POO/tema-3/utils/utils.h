#ifndef _UTILS_H_
#define _UTILS_H_

#include <type_traits>
#include <iostream>

namespace utils {

template<typename T>
class GenericGet {
  public:
    GenericGet(const T el=T()) : el_(el) {}

    template<typename... Args> GenericGet(Args&&... args) : el_(std::forward<Args>(args)...) {}

    GenericGet(const GenericGet&) = default;
    GenericGet(GenericGet&&) = default;
    GenericGet& operator =(const GenericGet&) = default;
    GenericGet& operator =(GenericGet&&) = default;
    virtual ~GenericGet() = default;

    T const& get() const { return el_; }
    T& get() { return el_; }

    template<typename U>
    friend std::ostream& operator <<(std::ostream& os, const GenericGet<U>& rhs) { return os << rhs.get(); }

    bool operator ==(const GenericGet& rhs) { return el_ == rhs.el_; }

  private:
    T el_;
};

}  // namespace utils

#endif  // _UTILS_H_
