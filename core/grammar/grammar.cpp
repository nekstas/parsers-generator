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
