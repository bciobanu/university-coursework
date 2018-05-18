#ifndef _SETS_MOD_NUMBER_H_
#define _SETS_MOD_NUMBER_H_

#include <iostream>

#include "../utils/utils.h"
#include "../base_set_datatype.h"

namespace crypto {

// CRTP
template<int mod>
class ModInteger : public BaseSetDataType<ModInteger<mod>> {
  public:
    ModInteger(int el=0);
    ModInteger& operator *=(const ModInteger&);
    ModInteger& operator +=(const ModInteger&);
    ModInteger& operator /=(const ModInteger&);
    ModInteger& operator -=(const ModInteger&);

    ModInteger Inverse() const;

    bool operator ==(const ModInteger&) const;

    template<int m>
    friend std::ostream& operator <<(std::ostream&, const ModInteger<m>&);

  private:
    int el_;
};

template<int mod>
class ModNumber : public utils::GenericGet<ModInteger<mod>> {
  public:
    using utils::GenericGet<ModInteger<mod>>::GenericGet;
};

}  // namespace crypto

#include "mod_number.cc"
#endif  // _SETS_MOD_NUMBER_H_