#pragma once

#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include "../data/identifiers.h"
#include "../lexer.h"
#include "grammar.h"

namespace pg {

struct LrAction {
    enum class Type { SHIFT, REDUCE, ACCEPT, ERROR };
    Type type;
    size_t index;
};

using LrActionTable = std::vector<std::map<TokenType, LrAction>>;
using LrGotoTable = std::vector<std::map<Identifier, size_t>>;

struct LrData {
    Grammar grammar;
    LrActionTable action_table;
    LrGotoTable goto_table;
};

}  // namespace pg
