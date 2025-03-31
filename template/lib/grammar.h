#pragma once

#include <variant>
#include <vector>

#include "../data/identifiers.h"
#include "../lexer.h"

namespace pg {

using Symbol = std::variant<TokenType, Identifier>;

struct Rule {
    Identifier result;
    std::string repr;
    std::vector<Symbol> sequence;
};

struct Grammar {
    std::vector<Rule> rules;
};

}  // namespace pg
