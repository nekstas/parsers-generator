#include "grammar_info.h"

#include "../../utils/input_output.hpp"

grammar::GrammarInfo::GrammarInfo(const grammar::Grammar& grammar) : grammar_(grammar) {
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
        // TODO: throw an error, that main rule is not selected
        throw std::runtime_error{"No main rule."};
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
            // TODO: throw an error.
            throw std::runtime_error{"Rule is useless."};
        }
    }

    for (const std::string& rule : used_rules_) {
        if (!rules_map.contains(rule)) {
            // TODO: throw an error.
            throw std::runtime_error{"Rule does not exist."};
        }
    }
}

void grammar::GrammarInfo::BuildFirstValues() {
    for (const std::string& token : used_tokens_) {
        first_[Symbol::MakeTerminal(token)] = {token};
    }
    first_[Symbol::MakeTerminal(kEmptyRuleName)] = {kEmptyRuleName};

    bool sets_changed = true;
    while (sets_changed) {
        sets_changed = false;
        for (const Rule& rule : grammar_.GetRules()) {
            auto& current = first_[Symbol::MakeNonTerminal(rule.name)];
            for (size_t i = 0; i < rule.sequence.size(); ++i) {
                sets_changed |= MoveTokens(first_[rule.sequence[i]], current);

                if (!first_[rule.sequence[i]].contains(kEmptyRuleName)) {
                    break;
                }
                if (i == rule.sequence.size() - 1) {
                    sets_changed = true;
                    current.insert(kEmptyRuleName);
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

                if (first_[rule.sequence[i - 1]].contains(kEmptyRuleName)) {
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
    bool result_changed = false;
    for (const std::string& token : values) {
        if (token != kEmptyRuleName && !result.contains(token)) {
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

std::ostream& operator<<(std::ostream& out, const grammar::GrammarInfo& info) {
    out << info.GetGrammar() << "\n---\n";
    out << "Used tokens: " << info.GetUsedTokens() << "\n";
    out << "Used rules: " << info.GetUsedRules() << "\n---\n";
    out << "[FIRST]:\n" << info.GetFirstMap() << "---\n";
    out << "[FOLLOW]:\n" << info.GetFollowMap();
    return out;
}
