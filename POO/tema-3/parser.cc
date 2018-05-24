#include "parser.h"

#include <cctype>     // isdigit
#include <algorithm>  // std::remove_if

namespace crypto {

template <typename T>
T Parser<T>::Evaluate(std::string s) {
    s.erase(std::remove_if(s.begin(), s.end(), [](char x) { return std::isspace(x); }), s.end());
    for (int i = 0; i < (int)s.length(); ++i) {
        if (s[i] == 'i') {
            int j = i - 1;
            while (j >= 0 and (isdigit(s[j]) or s[j] == '.')) {
                s[j + 1] = s[j];
                --j;
            }
            s[j + 1] = 'i';
        }
    }

    stream_.clear();
    stream_ << s;
    return Parse();
}

template <typename T>
T Parser<T>::Parse(Level l) {
    l = AdvanceLevel(l);
    T result;
    if (l == kNumber) {
        if (stream_.peek() == '(') {  // parenthesis expression
            stream_.get();  // '('
            result = Parse(l);
            stream_.get();  // ')'
        } else {  // parse a T
            stream_ >> result;
        }
    } else {
        #define W(S0, S1) { \
            result = Parse(l); \
            while (stream_.peek() == *#S0 or stream_.peek() == *#S1) { \
                if (stream_.get() == *#S0) { \
                    result S0##= Parse(l); \
                } else { \
                    result S1##= Parse(l); \
                } \
            } \
        }

        if (l == kPlus) {
            W(+, -)
        } else {
            W(*, /)
        }
        #undef W
    }
    return result;
}

}  // namespace crypto
