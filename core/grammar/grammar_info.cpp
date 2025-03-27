#include "grammar_info.h"

#include "../../utils/input_output.hpp"

grammar::GrammarInfo::GrammarInfo(const grammar::Grammar& grammar) : grammar_(grammar) {
    Build();
}

void grammar::GrammarInfo::Build() {
    UpdateMainRule();
    BuildUsedSets();

    // TODO: check all used rules exist and no extra rules
}

const grammar::Grammar& grammar::GrammarInfo::GetGrammar() const {
    return grammar_;
}

size_t grammar::GrammarInfo::GetMainRule() const {
    return grammar_.GetRulesCount() - 1;
}

const std::set<std::string>& grammar::GrammarInfo::GetUsedTokens() const {
    return used_tokens_;
}

const std::set<std::string>& grammar::GrammarInfo::GetUsedRules() const {
    return used_rules_;
}

void grammar::GrammarInfo::UpdateMainRule() {
    auto old_main_rule = grammar_.GetMainRule();
    if (old_main_rule.empty()) {
        // TODO: throw an error, that main rule is not selected
        throw std::runtime_error{"No main rule."};
    }

    grammar::Symbol symbol = {grammar::Symbol::Type::NonTerminal, old_main_rule};
    grammar_.AddRule({kNewMainRuleName, {symbol}});
    grammar_.SetMainRule(kNewMainRuleName);
}

void grammar::GrammarInfo::BuildUsedSets() {
    for (const grammar::Rule& rule : grammar_.GetRules()) {
        for (const grammar::Symbol& item : rule.sequence) {
            if (item.type == Symbol::Type::Terminal) {
                used_tokens_.insert(item.value);
            } else {
                used_rules_.insert(item.value);
            }
        }
    }

    used_tokens_.insert(kEofTokenName);
}

std::ostream& operator<<(std::ostream& out, const grammar::GrammarInfo& info) {
    out << info.GetGrammar();
    out << "Used tokens: " << info.GetUsedTokens() << "\n";
    out << "Used rules: " << info.GetUsedRules() << "\n";
    return out;
}
