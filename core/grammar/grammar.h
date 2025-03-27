#pragma once

#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

namespace grammar {

struct Symbol {
    enum class Type { Terminal, NonTerminal };

    Type type;
    std::string value;

    bool operator<(const Symbol& other) const;
    bool operator==(const Symbol& other) const;
    bool operator!=(const Symbol& other) const;
};

struct Rule {
    std::string name;
    std::vector<Symbol> sequence;
};

class Grammar {
public:
    size_t AddRule(const Rule& rule);
    const Rule& GetRule(size_t index) const;
    void SetMainRule(const std::string& name);
    std::string GetMainRule() const;
    size_t GetRulesCount() const;
    const std::vector<Rule>& GetRules() const;
    const std::vector<size_t>& GetRulesFor(const std::string& name) const;

private:
    std::vector<Rule> rules_;
    std::map<std::string, std::vector<size_t>> rules_for_name_;
    std::string main_rule_;
};

}  // namespace grammar

std::ostream& operator<<(std::ostream& out, const grammar::Symbol& item);
std::ostream& operator<<(std::ostream& out, const grammar::Rule& rule);
std::ostream& operator<<(std::ostream& out, const grammar::Grammar& grammar);
