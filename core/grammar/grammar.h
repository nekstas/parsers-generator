#pragma once

#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

namespace grammar {

struct SequenceItem {
    enum class Type { Terminal, NonTerminal };

    Type type;
    std::string value;
};

struct Rule {
    std::string name;
    std::vector<SequenceItem> sequence;
};

class Grammar {
public:
    size_t AddRule(const Rule& rule);

    size_t GetRulesCount() const;

    const std::vector<Rule>& GetRules() const;

private:
    std::vector<Rule> rules_;
    std::map<std::string, std::vector<size_t>> rules_for_name_;
};

std::ostream& operator<<(std::ostream& out, const SequenceItem& item);
std::ostream& operator<<(std::ostream& out, const Rule& rule);
std::ostream& operator<<(std::ostream& out, const Grammar& grammar);

}  // namespace grammar
