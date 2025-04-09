#pragma once

#include "../../grammar/grammar.h"
#include "../lib/ast_node.h"

namespace ast {

class Symbol : public ast::Node {
public:
    Symbol(grammar::Symbol symbol) : symbol_(symbol) {
    }

    grammar::Symbol GetSymbol() const {
        return symbol_;
    }

private:
    grammar::Symbol symbol_;
};

class TakeSymbol : public ast::Node {
public:
    TakeSymbol(grammar::Symbol symbol, bool take) : symbol_(symbol), take_(take) {
    }

    grammar::Symbol GetSymbol() const {
        return symbol_;
    }

    bool GetTake() const {
        return take_;
    }

private:
    grammar::Symbol symbol_;
    bool take_;
};

class SequenceOfSymbols : public ast::Node {
public:
    SequenceOfSymbols(std::shared_ptr<TakeSymbol> symbol)
        : sequence_{symbol->GetSymbol()}, take_(symbol->GetTake()) {
    }

    void AddSymbol(std::shared_ptr<TakeSymbol> symbol) {
        sequence_.push_back(symbol->GetSymbol());
        take_.push_back(symbol->GetTake());
    }

    const std::vector<grammar::Symbol>& GetSequence() const {
        return sequence_;
    }

    const std::vector<bool>& GetTake() const {
        return take_;
    }

private:
    std::vector<grammar::Symbol> sequence_;
    std::vector<bool> take_;
};

class Production : public ast::Node {
public:
    Production(const SequenceOfSymbols& sequence, const std::string& handler_name)
        : sequence_(sequence), handler_name_(handler_name) {
    }

    const std::string& GetHandlerName() const {
        return handler_name_;
    }

    const SequenceOfSymbols& GetSequence() const {
        return sequence_;
    }

private:
    SequenceOfSymbols sequence_;
    std::string handler_name_;
};

class ProductionList : public ast::Node {
public:
    ProductionList(const Production& production) : productions_{production} {
    }

    void AddProduction(const Production& production) {
        productions_.push_back(production);
    }

    const std::vector<Production>& GetProductions() const {
        return productions_;
    }

private:
    std::vector<Production> productions_;
};

class RuleHeader : public ast::Node {
public:
    RuleHeader(const std::string& rule_name, const std::string& return_type_name)
        : rule_name_(rule_name), return_type_name_(return_type_name) {
    }

    const std::string& GetRuleName() const {
        return rule_name_;
    }

    const std::string& GetReturnTypeName() const {
        return return_type_name_;
    }

private:
    std::string rule_name_;
    std::string return_type_name_;
};

class Rule : public ast::Node {
public:
    Rule(const RuleHeader& header, const std::vector<grammar::Rule>& rules)
        : header_(header), rules_(rules) {
    }

    RuleHeader GetRuleHeader() const {
        return header_;
    }

    const std::vector<grammar::Rule>& GetRules() const {
        return rules_;
    }

private:
    RuleHeader header_;
    std::vector<grammar::Rule> rules_;
};

class MainRule : public Rule {
public:
    using Rule::Rule;
};

class RuleList : public ast::Node {
public:
    RuleList(std::shared_ptr<Rule> rule) {
        AddRule(rule);
    }

    const grammar::Grammar& GetGrammar() const {
        return grammar_;
    }

    void AddRule(std::shared_ptr<Rule> rule) {
        // TODO: check rule does not exist
        const auto& header = rule->GetRuleHeader();
        grammar_.SetReturnType(header.GetRuleName(), header.GetReturnTypeName());
        for (const auto& grammar_rule : rule->GetRules()) {
            grammar_.AddRule(grammar_rule);
        }

        if (Is<MainRule>(rule)) {
            // TODO: check main rule does not exist
            grammar_.SetMainRule(header.GetRuleName());
        }
    }

private:
    grammar::Grammar grammar_;
};

}  // namespace ast
