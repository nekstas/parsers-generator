#pragma once
#include <vector>
#include "../lib/ast_node.h"
#include "../lib/grammar.h"
#include "../lib/token.h"
#include "../lib/tokenizer.h"
#include "../lib/error_printer.h"

#include "../usr/ast_nodes.h"

namespace ast {
class Grammar;
class Production;
class ProductionList;
class Rule;
class RuleHeader;
class RuleList;
class SequenceOfSymbols;
class Symbol;
class TakeSymbol;
}

namespace pg {
class AstBuilder {
public:
    virtual ~AstBuilder() {}

    virtual void Setup() {};
    virtual void Accept(std::shared_ptr<ast::Grammar> root) = 0;

    virtual std::shared_ptr<ast::Grammar> HandleGrammar(std::shared_ptr<ast::RuleList>) = 0;
    virtual std::shared_ptr<ast::Production> HandleProduction(std::shared_ptr<ast::SequenceOfSymbols>, const Token&) = 0;
    virtual std::shared_ptr<ast::ProductionList> HandleProductionListBegin(std::shared_ptr<ast::Production>) = 0;
    virtual std::shared_ptr<ast::ProductionList> HandleProductionListContinue(std::shared_ptr<ast::ProductionList>, std::shared_ptr<ast::Production>) = 0;
    virtual std::shared_ptr<ast::Rule> HandleCommonRule(std::shared_ptr<ast::RuleHeader>, std::shared_ptr<ast::ProductionList>) = 0;
    virtual std::shared_ptr<ast::Rule> HandleMainRule(std::shared_ptr<ast::RuleHeader>, std::shared_ptr<ast::ProductionList>) = 0;
    virtual std::shared_ptr<ast::RuleHeader> HandleCommonRuleHeader(const Token&) = 0;
    virtual std::shared_ptr<ast::RuleHeader> HandleTypedRuleHeader(const Token&, const Token&) = 0;
    virtual std::shared_ptr<ast::RuleList> HandleRuleListBegin(std::shared_ptr<ast::Rule>) = 0;
    virtual std::shared_ptr<ast::RuleList> HandleRuleListContinue(std::shared_ptr<ast::RuleList>, std::shared_ptr<ast::Rule>) = 0;
    virtual std::shared_ptr<ast::SequenceOfSymbols> HandleSequenceOfSymbolsBegin(std::shared_ptr<ast::TakeSymbol>) = 0;
    virtual std::shared_ptr<ast::SequenceOfSymbols> HandleSequenceOfSymbolsContinue(std::shared_ptr<ast::SequenceOfSymbols>, std::shared_ptr<ast::TakeSymbol>) = 0;
    virtual std::shared_ptr<ast::Symbol> HandleTerminal(const Token&) = 0;
    virtual std::shared_ptr<ast::Symbol> HandleNonTerminal(const Token&) = 0;
    virtual std::shared_ptr<ast::TakeSymbol> HandleDontTakeSymbol(std::shared_ptr<ast::Symbol>) = 0;
    virtual std::shared_ptr<ast::TakeSymbol> HandleTakeSymbol(std::shared_ptr<ast::Symbol>) = 0;
};
}