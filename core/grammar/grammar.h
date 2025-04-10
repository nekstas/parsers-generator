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

    static Symbol MakeTerminal(const std::string& name);
    static Symbol MakeNonTerminal(const std::string& name);
};

struct Rule {
    std::string name;
    std::string handler_name;
    std::vector<Symbol> sequence;
    std::vector<bool> take;

    Rule(const std::string& name, const std::string& handler_name,
         const std::vector<Symbol>& sequence, const std::vector<bool>& take);
};

class Grammar {
public:
    static constexpr std::string kEpsilonProduction = "<>";

public:
    size_t AddRule(const Rule& rule);
    bool HasRule(const std::string& name);
    const Rule& GetRule(size_t index) const;
    void SetMainRule(const std::string& name);
    void SetReturnType(const std::string& name, const std::string& return_type);
    std::string GetReturnType(const std::string& name) const;
    std::string GetMainRule() const;
    size_t GetRulesCount() const;
    const std::vector<Rule>& GetRules() const;
    const std::vector<size_t>& GetRulesFor(const std::string& name) const;
    const std::map<std::string, std::vector<size_t>>& GetRulesMap() const;

private:
    std::vector<Rule> rules_;
    std::map<std::string, std::vector<size_t>> rules_for_name_;
    std::string main_rule_;
    std::map<std::string, std::string> return_types_;
};

}  // namespace grammar

std::ostream& operator<<(std::ostream& out, const grammar::Symbol& item);
std::ostream& operator<<(std::ostream& out, const grammar::Rule& rule);
std::ostream& operator<<(std::ostream& out, const grammar::Grammar& grammar);
