#include "field/rational.h"
#include <bits/stdc++.h>

using namespace std;

int main() {
    crypto::RationalFieldElement x(1);
    crypto::RationalFieldElement y(1);
    cout << x << ' ' << y << endl;
    cout << boolalpha << (x == y) << endl;
    return 0;
}