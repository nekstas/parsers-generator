#pragma once

#include <variant>
#include <vector>

#include "../data/identifiers.h"
#include "../lexer.h"

namespace pg {

using Symbol = std::variant<Token, Identifier>;

struct Rule {
    Identifier result;
    std::vector<Symbol> sequence;
};

struct Grammar {
    std::vector<Rule> rules;
};

}  // namespace pg
