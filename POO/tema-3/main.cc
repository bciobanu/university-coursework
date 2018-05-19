#include "field/fields.h"
#include "parser.h"
#include <iostream>  // cout, cin
#include <iomanip>   // setw
#include <string>    // getline

using namespace std;

constexpr int kNumOptions = 7;
constexpr char kOptions[kNumOptions][21] = {
    "Iesire",
    "Operatii pe C",
    "Operatii pe R",
    "Operatii pe Q",
    "Operatii pe Z",
    "Operatii pe Z/nZ",
    "Operatii pe corp dat"
};

void ClearTerminal() {
    cout << "\033c";
}

int main() {
    while (true) {
        cout << "Optiuni:" << endl;
        for (int i = 0; i < kNumOptions; ++i) {
            cout << right << setw(10) << '(' << i + 1 << ')' << setw(22) << kOptions[i] << endl;
        }

        int option; cin >> option;
        ClearTerminal();
        if (option <= 0 or option > kNumOptions) {
            cout << "Optiune invalida..." << endl;
            continue;
        }

        if (option == 1) {
            break;
        }

        if (option == 6) {
            int n;
            cout << "n = ? "; cout.flush();
            cin >> n;
            try {
                crypto::ModInteger::SetModulo(n);
            } catch (exception& e) {
                ClearTerminal();
                cout << e.what() << endl;
                continue;
            }
        } else if (option == 7) {
            cout << "Ordinul corpului = ? "; cout.flush();
            int n; cin >> n;
            vector<vector<int>> plus(n, vector<int>(n)), times(n, vector<int>(n));
            cout << "Scrieti tabela adunarii (NxN)" << endl;
            for (int i = 0; i < n; ++i) {
                for (int j = 0; j < n; ++j) {
                    cin >> plus[i][j];
                }
            }

            cout << "Scrieti tabela inmultirii (NxN)" << endl;
            for (int i = 0; i < n; ++i) {
                for (int j = 0; j < n; ++j) {
                    cin >> times[i][j];
                }
            }

            try {
                crypto::FiniteElement::Init(plus, times);
            } catch (exception& e) {
                ClearTerminal();
                cout << e.what() << endl;
                continue;
            }
        }

        ClearTerminal();
        cout << "Scrieti o expresie sau \"exit\" daca vreti sa iesiti" << endl;
        while (true) {
            string expression;
            do {
                getline(cin, expression);
            } while (expression.empty() or expression.front() == '\n');

            if (expression.find("exit") != string::npos) {
                break;
            }

            try {
                cout << "= ";
                #define ADD(CLASS, T, OPTION) \
                case OPTION: \
                    static crypto::Parser<crypto::CLASS##FieldElement T> __parser__##OPTION; \
                    cout << __parser__##OPTION.Evaluate(expression); \
                    break

                switch (option) {
                    ADD(Complex, <double>, 2);
                    ADD(Real, <double>, 3);
                    ADD(Rational, <long long>, 4);
                    ADD(Integer, <long long>, 5);
                    ADD(Mod, , 6);
                    ADD(Finite, , 7);
                }
                #undef ADD

                cout << endl;
            } catch (exception& e) {
                cout << "\rOops.. something went wrong" << endl;
            }
        }

    }
    return 0;
}
