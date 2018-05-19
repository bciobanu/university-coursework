#ifndef _PARSER_H_
#define _PARSER_H_

#include <string>
#include <sstream>

namespace crypto {

template <typename T>
class Parser {
  public:
    T Evaluate(std::string);
  private:
    enum Level {
        kPlus   = 0,
        kTimes  = 1,
        kNumber = 2
    };

    static Level AdvanceLevel(const Level l) {
        return static_cast<Level>((l + 1) % 3);
    }

    T Parse(Level=kNumber);

    std::stringstream stream_;
};

}  // namespace crypto

#include "parser.cc"
#endif  // _PARSER_H_