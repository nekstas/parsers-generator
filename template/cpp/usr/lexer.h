#pragma once

#include <cstdint>
#include <string>

#include "../data/token_type.h"

namespace pg {

class Lexer {
public:
    TokenType GetTokenType(const char*& current, const char* end);
};

}  // namespace pg
