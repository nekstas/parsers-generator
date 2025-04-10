#pragma once

#include "../data/ast_builder.h"

class GrammarBuilder : public pg::AstBuilder {
public:
    grammar::Grammar GetResult() const;

public:
    void Accept(std::shared_ptr<ast::RuleList> root) override;
    std::shared_ptr<ast::Production> HandleProduction(std::shared_ptr<ast::SequenceOfSymbols>,
                                                      const pg::Token&) override;
    std::shared_ptr<ast::Production> HandleEpsProduction(const pg::Token& token) override;
    std::shared_ptr<ast::ProductionList> HandleProductionListBegin(
        std::shared_ptr<ast::Production>) override;
    std::shared_ptr<ast::ProductionList> HandleProductionListContinue(
        std::shared_ptr<ast::ProductionList>, std::shared_ptr<ast::Production>) override;
    std::shared_ptr<ast::Rule> HandleCommonRule(std::shared_ptr<ast::RuleHeader>,
                                                std::shared_ptr<ast::ProductionList>) override;
    std::shared_ptr<ast::Rule> HandleMainRule(std::shared_ptr<ast::RuleHeader>,
                                              std::shared_ptr<ast::ProductionList>) override;
    std::shared_ptr<ast::RuleHeader> HandleCommonRuleHeader(const pg::Token&) override;
    std::shared_ptr<ast::RuleHeader> HandleTypedRuleHeader(const pg::Token&,
                                                           const pg::Token&) override;
    std::shared_ptr<ast::RuleList> HandleRuleListBegin(std::shared_ptr<ast::Rule>) override;
    std::shared_ptr<ast::RuleList> HandleRuleListContinue(std::shared_ptr<ast::RuleList>,
                                                          std::shared_ptr<ast::Rule>) override;
    std::shared_ptr<ast::SequenceOfSymbols> HandleSequenceOfSymbolsBegin(
        std::shared_ptr<ast::TakeSymbol>) override;
    std::shared_ptr<ast::SequenceOfSymbols> HandleSequenceOfSymbolsContinue(
        std::shared_ptr<ast::SequenceOfSymbols>, std::shared_ptr<ast::TakeSymbol>) override;
    std::shared_ptr<ast::Symbol> HandleTerminal(const pg::Token&) override;
    std::shared_ptr<ast::Symbol> HandleNonTerminal(const pg::Token&) override;
    std::shared_ptr<ast::TakeSymbol> HandleDontTakeSymbol(std::shared_ptr<ast::Symbol>) override;
    std::shared_ptr<ast::TakeSymbol> HandleTakeSymbol(std::shared_ptr<ast::Symbol>) override;

private:
    std::vector<grammar::Rule> ProcessProductions(const ast::RuleHeader& header,
                                                  std::vector<ast::Production> productions);

private:
    grammar::Grammar result_;
};
