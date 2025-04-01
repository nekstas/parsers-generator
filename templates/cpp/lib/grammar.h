#pragma once

#include <functional>
#include <variant>
#include <vector>

#include "../data/identifier.h"
#include "../lib/ast_node.h"
#include "../lib/token.h"
#include "../usr/lexer.h"

namespace pg {

using Symbol = std::variant<TokenType, Identifier>;

class AstBuilder;

struct Rule {
    using HandlerArg = std::variant<Token, ast::AstNodePtr>;
    using HandlerArgs = std::vector<HandlerArg>;
    using HandlerType =
        std::function<ast::AstNodePtr(AstBuilder& ast_builder, const HandlerArgs& args)>;

    Identifier result;
    std::vector<Symbol> sequence;
    HandlerType handler;
};

struct Grammar {
    std::vector<Rule> rules;
};

}  // namespace pg
