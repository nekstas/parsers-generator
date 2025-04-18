#include "grammar_info.h"

#include "../../utils/format_stream.h"
#include "../../utils/input_output.hpp"
#include "../app/errors.h"

grammar::GrammarInfo::GrammarInfo(const grammar::Grammar& grammar)
    : grammar_(grammar), old_main_rule_name_(grammar.GetMainRule()) {
    Build();
}

void grammar::GrammarInfo::Build() {
    UpdateMainRule();
    BuildUsedSets();
    CheckUsedRules();
    BuildFirstValues();
    BuildFollowValues();
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
        throw errors::ApplicationError{
            "There are no main rule in grammar. Use special syntax to mark main rule."};
    }

    grammar::Symbol symbol = {grammar::Symbol::Type::NonTerminal, old_main_rule};
    grammar_.AddRule({kNewMainRuleName, "", {symbol}, {true}});
    grammar_.SetMainRule(kNewMainRuleName);
}

void grammar::GrammarInfo::BuildUsedSets() {
    for (const grammar::Rule& rule : grammar_.GetRules()) {
        AddUsedReturnTypeFor(rule.name);
        for (const grammar::Symbol& symbol : rule.sequence) {
            AddUsedSymbol(symbol);
        }
        AddUsedHandlerName(rule.handler_name);
    }

    AddUsedToken(kEofTokenName);
    AddUsedRule(kNewMainRuleName);
}

void grammar::GrammarInfo::AddUsedToken(const std::string& name) {
    used_tokens_.insert(name);
    used_symbols_.insert({Symbol::Type::Terminal, name});
}

void grammar::GrammarInfo::AddUsedRule(const std::string& name) {
    used_rules_.insert(name);
    used_symbols_.insert({Symbol::Type::NonTerminal, name});
}

void grammar::GrammarInfo::AddUsedSymbol(const grammar::Symbol& symbol) {
    if (symbol.type == Symbol::Type::Terminal) {
        AddUsedToken(symbol.value);
    } else {
        AddUsedRule(symbol.value);
    }
}

const std::set<grammar::Symbol>& grammar::GrammarInfo::GetUsedSymbols() const {
    return used_symbols_;
}

void grammar::GrammarInfo::CheckUsedRules() {
    const auto& rules_map = grammar_.GetRulesMap();

    for (const auto& [name, rules] : rules_map) {
        if (!used_rules_.contains(name)) {
            throw errors::ApplicationError{FormatStream() << "Rule with name \"" << name
                                                          << "\" is not used anywhere."};
        }
    }

    for (const std::string& rule : used_rules_) {
        if (!rules_map.contains(rule)) {
            throw errors::ApplicationError{
                FormatStream() << "A production use undefined rule with name \"" << rule << "\"."};
        }
    }
}

void grammar::GrammarInfo::BuildFirstValues() {
    for (const std::string& token : used_tokens_) {
        first_[Symbol::MakeTerminal(token)] = {token};
    }

    bool sets_changed = true;
    while (sets_changed) {
        sets_changed = false;
        for (const Rule& rule : grammar_.GetRules()) {
            auto nt = Symbol::MakeNonTerminal(rule.name);
            if (rule.sequence.empty()) {
                if (!first_[nt].contains(kEpsilon)) {
                    first_[nt].insert(kEpsilon);
                    sets_changed = true;
                }
                continue;
            }

            for (size_t i = 0; i < rule.sequence.size(); ++i) {
                sets_changed |= MoveTokens(first_[rule.sequence[i]], first_[nt]);

                if (!first_[rule.sequence[i]].contains(kEpsilon)) {
                    break;
                }
                if (i == rule.sequence.size() - 1 && !first_[nt].contains(kEpsilon)) {
                    sets_changed = true;
                    first_[nt].insert(kEpsilon);
                }
            }
        }
    }
}

void grammar::GrammarInfo::BuildFollowValues() {
    follow_[kNewMainRuleName] = {kEofTokenName};

    bool sets_changed = true;
    while (sets_changed) {
        sets_changed = false;
        for (const Rule& rule : grammar_.GetRules()) {
            std::set<std::string> buffer = follow_[rule.name];
            for (size_t i = rule.sequence.size(); i > 0; --i) {
                if (rule.sequence[i - 1].type == Symbol::Type::NonTerminal) {
                    sets_changed |= MoveTokens(buffer, follow_[rule.sequence[i - 1].value]);
                }

                if (first_[rule.sequence[i - 1]].contains(kEpsilon)) {
                    MoveTokens(first_[rule.sequence[i - 1]], buffer);
                } else {
                    buffer = first_[rule.sequence[i - 1]];
                }
            }
        }
    }
}

const std::map<grammar::Symbol, std::set<std::string>>& grammar::GrammarInfo::GetFirstMap() const {
    return first_;
}

const std::map<std::string, std::set<std::string>>& grammar::GrammarInfo::GetFollowMap() const {
    return follow_;
}

bool grammar::GrammarInfo::MoveTokens(const std::set<std::string>& values,
                                      std::set<std::string>& result) {
    if (&values == &result) {
        return false;
    }
    bool result_changed = false;
    for (const std::string& token : values) {
        if (token != kEpsilon && !result.contains(token)) {
            result_changed = true;
            result.insert(token);
        }
    }
    return result_changed;
}

const std::set<std::string>& grammar::GrammarInfo::GetFollow(const std::string& name) const {
    return follow_.at(name);
}

void grammar::GrammarInfo::AddUsedReturnTypeFor(const std::string& name) {
    if (name != kNewMainRuleName) {
        used_return_types_.insert(grammar_.GetReturnType(name));
    }
}

const std::set<std::string>& grammar::GrammarInfo::GetUsedReturnTypes() const {
    return used_return_types_;
}

const std::string grammar::GrammarInfo::GetOldMainRuleName() const {
    size_t new_main_rule_index = grammar_.GetRulesFor(grammar_.GetMainRule()).at(0);
    return grammar_.GetRule(new_main_rule_index).sequence.at(0).value;
}

void grammar::GrammarInfo::AddUsedHandlerName(const std::string& handler_name) {
    if (used_handler_names_.contains(handler_name)) {
        throw errors::ApplicationError{FormatStream() << "The same handler name \"" << handler_name
                                                      << "\" for different productions."};
    }

    used_handler_names_.insert(handler_name);
}

std::ostream& operator<<(std::ostream& out, const grammar::GrammarInfo& info) {
    out << info.GetGrammar() << "\n---\n";
    out << "Used tokens: " << info.GetUsedTokens() << "\n";
    out << "Used rules: " << info.GetUsedRules() << "\n---\n";
    out << "[FIRST]:\n" << info.GetFirstMap() << "---\n";
    out << "[FOLLOW]:\n" << info.GetFollowMap();
    return out;
}
