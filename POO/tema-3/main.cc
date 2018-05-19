#include "field/fields.h"
#include <bits/stdc++.h>

using namespace std;

int main() {
    vector<vector<int>> plus = {{0, 1}, {1, 0}};
    vector<vector<int>> times = {{0, 0}, {0, 1}};

    crypto::FiniteElement::Init(plus, times);

    crypto::FiniteFieldElement x(1);
    crypto::FiniteFieldElement y(1);
    x *= y;
    cout << x << endl;

    return 0;
}