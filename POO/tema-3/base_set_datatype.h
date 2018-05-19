#ifndef _BASE_SET_DATATYPE_H_
#define _BASE_SET_DATATYPE_H_

namespace crypto {

template <typename DerivedDataType>
class BaseSetDataType {
  public:
    virtual DerivedDataType& operator +=(const DerivedDataType&) = 0;
    virtual DerivedDataType& operator -=(const DerivedDataType&) = 0;
    virtual DerivedDataType& operator *=(const DerivedDataType&) = 0;
    virtual DerivedDataType& operator /=(const DerivedDataType&) = 0;
};

}  // namespace crypto

#include "base_set_datatype.cc"
#endif  // _BASE_SET_DATATYPE_H_