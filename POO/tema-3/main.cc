#include "field/generic.h"
#include "field/fields.h"
#include "sets/rational_number.h"
#include "base_set_datatype.h"
#include "utils/utils.h"

#include <bits/stdc++.h>

using namespace std;

int main() {
    crypto::IntegerFieldElement<int> x(10);
    crypto::IntegerFieldElement<int> y(x);
    x += y;
    return 0;
}