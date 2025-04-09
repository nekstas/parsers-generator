#pragma once

#include "../data/ast_builder.h"

class GrammarBuilder : public pg::AstBuilder {
public:
    grammar::Grammar GetResult() const;

public:
    void Accept(std::shared_ptr<ast::RuleList> root) override;
    std::shared_ptr<ast::Production> HandleProduction(std::shared_ptr<ast::SequenceOfSymbols> ptr,
                                                      const pg::Token& token) override;
    std::shared_ptr<ast::ProductionList> HandleProductionListBegin(
        std::shared_ptr<ast::Production> ptr) override;
    std::shared_ptr<ast::ProductionList> HandleProductionListContinue(
        std::shared_ptr<ast::ProductionList> ptr,
        std::shared_ptr<ast::Production> sharedPtr) override;
    std::shared_ptr<ast::Rule> HandleCommonRule(
        std::shared_ptr<ast::RuleHeader> ptr,
        std::shared_ptr<ast::ProductionList> sharedPtr) override;
    std::shared_ptr<ast::Rule> HandleMainRule(
        std::shared_ptr<ast::RuleHeader> ptr,
        std::shared_ptr<ast::ProductionList> sharedPtr) override;
    std::shared_ptr<ast::RuleHeader> HandleCommonRuleHeader(const pg::Token& token) override;
    std::shared_ptr<ast::RuleHeader> HandleTypedRuleHeader(const pg::Token& token,
                                                           const pg::Token& token1) override;
    std::shared_ptr<ast::RuleList> HandleRuleListBegin(std::shared_ptr<ast::Rule> ptr) override;
    std::shared_ptr<ast::RuleList> HandleRuleListContinue(
        std::shared_ptr<ast::RuleList> ptr, std::shared_ptr<ast::Rule> sharedPtr) override;
    std::shared_ptr<ast::SequenceOfSymbols> HandleSequenceOfSymbolsBegin(
        std::shared_ptr<ast::TakeSymbol> ptr) override;
    std::shared_ptr<ast::SequenceOfSymbols> HandleSequenceOfSymbolsContinue(
        std::shared_ptr<ast::SequenceOfSymbols> ptr,
        std::shared_ptr<ast::TakeSymbol> sharedPtr) override;
    std::shared_ptr<ast::Symbol> HandleTerminal(const pg::Token& token) override;
    std::shared_ptr<ast::Symbol> HandleNonTerminal(const pg::Token& token) override;
    std::shared_ptr<ast::TakeSymbol> HandleDontTakeSymbol(
        std::shared_ptr<ast::Symbol> ptr) override;
    std::shared_ptr<ast::TakeSymbol> HandleTakeSymbol(std::shared_ptr<ast::Symbol> ptr) override;

private:
    std::vector<grammar::Rule> ProcessProductions(const ast::RuleHeader& header,
                                                  std::vector<ast::Production> productions);

private:
    grammar::Grammar result_;
};
