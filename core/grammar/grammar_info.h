#pragma once

#include <set>

#include "grammar.h"

namespace grammar {

class GrammarInfo {
public:
    static constexpr std::string kEofTokenName = "Eof";
    static constexpr std::string kNewMainRuleName = "__main__";

public:
    GrammarInfo(const Grammar& grammar);

    const Grammar& GetGrammar() const;
    const std::set<std::string>& GetUsedTokens() const;
    const std::set<std::string>& GetUsedRules() const;

private:
    void Build();
    void UpdateMainRule();
    void BuildUsedSets();

private:
    Grammar grammar_;
    std::set<std::string> used_tokens_;
    std::set<std::string> used_rules_;
};

}  // namespace grammar

std::ostream& operator<<(std::ostream& out, const grammar::GrammarInfo& info);
