#include "field/real.h"
#include <bits/stdc++.h>

using namespace std;

int main() {
    crypto::RealFieldElement x(5);
    crypto::RealFieldElement y(5);
    x += y;
    cout << x << ' ' << y << endl;
    return 0;
}