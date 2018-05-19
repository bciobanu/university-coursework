#ifndef _SETS_FINITE_H_
#define _SETS_FINITE_H_

#include <iostream>
#include <vector>

#include "../utils/utils.h"
#include "../base_set_datatype.h"

namespace crypto {

class FiniteElement : public BaseSetDataType<FiniteElement> {
  public:
    using IntVector = std::vector<int>;
    using IntMatrix = std::vector<IntVector>;

    static void Init(const IntMatrix&, const IntMatrix&);
    static int Cardinal();

    FiniteElement(int=zero_);

    FiniteElement& operator +=(const FiniteElement&);
    FiniteElement& operator -=(const FiniteElement&);
    FiniteElement& operator *=(const FiniteElement&);
    FiniteElement& operator /=(const FiniteElement&);

    bool operator ==(const FiniteElement&);

    friend std::ostream& operator <<(std::ostream&, const FiniteElement&);
    friend std::istream& operator >>(std::istream&, FiniteElement&);

  private:
    static IntMatrix plus_, times_;
    static IntVector inverse_plus_, inverse_times_;
    static int zero_;
    int el_;
};

class FiniteNumber : public utils::GenericGet<FiniteElement> {
  public:
    using utils::GenericGet<FiniteElement>::GenericGet;
};

}  // namespace crypto

#include "finite.cc"
#endif  // _SETS_FINITE_H_