#include "grammar.h"

size_t grammar::Grammar::AddRule(const grammar::Rule& rule) {
    // TODO: add check if rule already exist
    size_t index = GetRulesCount();
    rules_.push_back(rule);
    rules_for_name_[rule.name].push_back(index);
    return index;
}

size_t grammar::Grammar::GetRulesCount() const {
    return rules_.size();
}

const std::vector<grammar::Rule>& grammar::Grammar::GetRules() const {
    return rules_;
}

void grammar::Grammar::SetMainRule(const std::string& name) {
    main_rule_ = name;
}

std::string grammar::Grammar::GetMainRule() const {
    return main_rule_;
}

const grammar::Rule& grammar::Grammar::GetRule(size_t index) const {
    return rules_.at(index);
}

const std::vector<size_t>& grammar::Grammar::GetRulesFor(const std::string& name) const {
    return rules_for_name_.at(name);
}

const std::map<std::string, std::vector<size_t>>& grammar::Grammar::GetRulesMap() const {
    return rules_for_name_;
}

void grammar::Grammar::SetReturnType(const std::string& name, const std::string& return_type) {
    if (return_types_.contains(name) && return_types_.at(name) != return_type) {
        // TODO: throw an error
        throw std::runtime_error{
            "Return type should be the same for every rule for the same identifier."};
    }
    return_types_[name] = return_type;
}

std::string grammar::Grammar::GetReturnType(const std::string& name) const {
    return return_types_.at(name);
}

std::ostream& operator<<(std::ostream& out, const grammar::Symbol& symbol) {
    if (symbol.type == grammar::Symbol::Type::Terminal) {
        return out << symbol.value;
    }
    return out << "<" << symbol.value << ">";
}

std::ostream& operator<<(std::ostream& out, const grammar::Rule& rule) {
    out << "<" << rule.name << ">" << " ::=";
    for (const grammar::Symbol& symbol : rule.sequence) {
        out << " " << symbol;
    }
    return out;
}

std::ostream& operator<<(std::ostream& out, const grammar::Grammar& grammar) {
    out << "Grammar [" << grammar.GetRulesCount();
    out << " rules, main rule <" << grammar.GetMainRule() << ">]:\n";
    for (const grammar::Rule& rule : grammar.GetRules()) {
        out << rule << "\n";
    }
    return out;
}

bool grammar::Symbol::operator<(const grammar::Symbol& other) const {
    return std::tie(type, value) < std::tie(other.type, other.value);
}

bool grammar::Symbol::operator==(const grammar::Symbol& other) const {
    return std::tie(type, value) == std::tie(other.type, other.value);
}

bool grammar::Symbol::operator!=(const grammar::Symbol& other) const {
    return !(*this == other);
}

grammar::Symbol grammar::Symbol::MakeTerminal(const std::string& name) {
    return {Symbol::Type::Terminal, name};
}

grammar::Symbol grammar::Symbol::MakeNonTerminal(const std::string& name) {
    return {Symbol::Type::NonTerminal, name};
}

grammar::Rule::Rule(const std::string& name, const std::string& handler_name,
                    const std::vector<Symbol>& sequence, const std::vector<bool>& take)
    : name(name), handler_name(handler_name), sequence(sequence), take(take) {
}
