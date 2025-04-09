#include "grammar_builder.h"

void GrammarBuilder::Accept(std::shared_ptr<ast::RuleList> root) {
    result_ = root->GetGrammar();
}

std::shared_ptr<ast::Symbol> GrammarBuilder::HandleTerminal(const pg::Token& name) {
    return ast::Make<ast::Symbol>(grammar::Symbol{grammar::Symbol::Type::Terminal, name.value});
}

std::shared_ptr<ast::Symbol> GrammarBuilder::HandleNonTerminal(const pg::Token& name) {
    return ast::Make<ast::Symbol>(grammar::Symbol{grammar::Symbol::Type::NonTerminal, name.value});
}

std::shared_ptr<ast::TakeSymbol> GrammarBuilder::HandleDontTakeSymbol(
    std::shared_ptr<ast::Symbol> symbol) {
    return ast::Make<ast::TakeSymbol>(symbol->GetSymbol(), false);
}

std::shared_ptr<ast::TakeSymbol> GrammarBuilder::HandleTakeSymbol(
    std::shared_ptr<ast::Symbol> symbol) {
    return ast::Make<ast::TakeSymbol>(symbol->GetSymbol(), true);
}

std::shared_ptr<ast::SequenceOfSymbols> GrammarBuilder::HandleSequenceOfSymbolsBegin(
    std::shared_ptr<ast::TakeSymbol> symbol) {
    return ast::Make<ast::SequenceOfSymbols>(symbol);
}

std::shared_ptr<ast::SequenceOfSymbols> GrammarBuilder::HandleSequenceOfSymbolsContinue(
    std::shared_ptr<ast::SequenceOfSymbols> sequence, std::shared_ptr<ast::TakeSymbol> symbol) {
    sequence->AddSymbol(symbol);
    return sequence;
}

std::shared_ptr<ast::Production> GrammarBuilder::HandleProduction(
    std::shared_ptr<ast::SequenceOfSymbols> sequence, const pg::Token& name) {
    return ast::Make<ast::Production>(*sequence, name.value);
}

std::shared_ptr<ast::ProductionList> GrammarBuilder::HandleProductionListBegin(
    std::shared_ptr<ast::Production> production) {
    return ast::Make<ast::ProductionList>(*production);
}

std::shared_ptr<ast::ProductionList> GrammarBuilder::HandleProductionListContinue(
    std::shared_ptr<ast::ProductionList> production_list,
    std::shared_ptr<ast::Production> production) {
    production_list->AddProduction(*production);
    return production_list;
}

std::shared_ptr<ast::RuleHeader> GrammarBuilder::HandleCommonRuleHeader(const pg::Token& name) {
    return ast::Make<ast::RuleHeader>(name.value, name.value);
}

std::shared_ptr<ast::RuleHeader> GrammarBuilder::HandleTypedRuleHeader(
    const pg::Token& rule_name, const pg::Token& return_type_name) {
    return ast::Make<ast::RuleHeader>(rule_name.value, return_type_name.value);
}

std::shared_ptr<ast::Rule> GrammarBuilder::HandleCommonRule(
    std::shared_ptr<ast::RuleHeader> header, std::shared_ptr<ast::ProductionList> productions) {
    return ast::Make<ast::Rule>(*header, ProcessProductions(*header, productions->GetProductions()));
}

std::shared_ptr<ast::Rule> GrammarBuilder::HandleMainRule(
    std::shared_ptr<ast::RuleHeader> header, std::shared_ptr<ast::ProductionList> productions) {
    return ast::Make<ast::MainRule>(*header, ProcessProductions(*header, productions->GetProductions()));
}

std::shared_ptr<ast::RuleList> GrammarBuilder::HandleRuleListBegin(std::shared_ptr<ast::Rule> rule) {
    return ast::Make<ast::RuleList>(rule);
}

std::shared_ptr<ast::RuleList> GrammarBuilder::HandleRuleListContinue(
    std::shared_ptr<ast::RuleList> rule_list, std::shared_ptr<ast::Rule> rule) {
    rule_list->AddRule(rule);
    return rule_list;
}

std::vector<grammar::Rule> GrammarBuilder::ProcessProductions(
    const ast::RuleHeader& header, std::vector<ast::Production> productions) {
    std::vector<grammar::Rule> result;
    for (const auto& production : productions) {
        const auto& sequence = production.GetSequence();
        result.emplace_back(header.GetRuleName(), production.GetHandlerName(),
                            sequence.GetSequence(), sequence.GetTake());
    }
    return result;
}

grammar::Grammar GrammarBuilder::GetResult() const {
    return result_;
}
