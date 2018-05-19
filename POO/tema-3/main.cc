#include "field/fields.h"
#include <bits/stdc++.h>

using namespace std;

int main() {
    crypto::ModInteger::SetModulo(666013);
    crypto::ModFieldElement x(10);
    crypto::ModFieldElement y(x);
    x += y;
    cout << x << endl;
    return 0;
}