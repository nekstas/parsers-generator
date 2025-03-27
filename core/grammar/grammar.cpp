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

std::ostream& operator<<(std::ostream& out, const grammar::SequenceItem& item) {
    if (item.type == grammar::SequenceItem::Type::Terminal) {
        return out << item.value;
    }
    return out << "<" << item.value << ">";
}

std::ostream& operator<<(std::ostream& out, const grammar::Rule& rule) {
    out << "<" << rule.name << ">" << " ::=";
    for (const grammar::SequenceItem& item : rule.sequence) {
        out << " " << item;
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
