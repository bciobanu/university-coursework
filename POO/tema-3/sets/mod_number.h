#ifndef _SETS_MOD_NUMBER_H_
#define _SETS_MOD_NUMBER_H_

#include <iostream>

#include "../utils/utils.h"
#include "../base_set_datatype.h"

namespace crypto {

class ModInteger : public BaseSetDataType<ModInteger> {
  public:
    ModInteger(int=0);
    ModInteger& operator *=(const ModInteger&);
    ModInteger& operator +=(const ModInteger&);
    ModInteger& operator /=(const ModInteger&);
    ModInteger& operator -=(const ModInteger&);

    ModInteger Inverse() const;
    ModInteger operator -() const;

    bool operator ==(const ModInteger&) const;

    friend std::ostream& operator <<(std::ostream&, const ModInteger&);
    friend std::istream& operator >>(std::istream&, ModInteger&);

    static void SetModulo(int);

  private:
    static int mod_;
    int el_;
};

class ModNumber : public utils::GenericGet<ModInteger> {
  public:
    using utils::GenericGet<ModInteger>::GenericGet;
};

}  // namespace crypto

#include "mod_number.cc"
#endif  // _SETS_MOD_NUMBER_H_