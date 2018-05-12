#include "field/real.h"
#include <bits/stdc++.h>

using namespace std;

int main() {
    crypto::RealFieldElement x(5);
    crypto::RealFieldElement y(0.1);
    x += y;
    x *= y;
    cout << x << ' ' << y << endl;
    cout << boolalpha << (x == y) << endl;
    return 0;
}