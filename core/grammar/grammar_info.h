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
    size_t GetMainRule() const;
    const std::set<std::string>& GetUsedTokens() const;
    const std::set<std::string>& GetUsedRules() const;
    const std::set<Symbol>& GetUsedSymbols() const;

private:
    void Build();
    void UpdateMainRule();
    void AddUsedToken(const std::string& name);
    void AddUsedRule(const std::string& name);
    void AddUsedSymbol(const Symbol& symbol);
    void BuildUsedSets();

private:
    Grammar grammar_;

    std::set<std::string> used_tokens_;
    std::set<std::string> used_rules_;
    std::set<Symbol> used_symbols_;

    std::map<Symbol, std::set<std::string>> first_;
    std::map<Symbol, std::set<std::string>> follow_;
};

}  // namespace grammar

std::ostream& operator<<(std::ostream& out, const grammar::GrammarInfo& info);
