#pragma once

#include <cstdint>
#include <string>

#include "../data/token_type.h"

namespace pg {

struct Token {
    TokenType type;
    std::string value;
    size_t line;
    size_t pos;

    Token(TokenType type, const std::string& value, size_t line, size_t pos)
        : type(type), value(value), line(line), pos(pos) {
    }
};

}  // namespace pg
