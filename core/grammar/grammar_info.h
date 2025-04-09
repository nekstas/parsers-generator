#pragma once

#include <set>

#include "grammar.h"

namespace grammar {

class GrammarInfo {
public:
    static constexpr std::string kEofTokenName = "Eof";
    static constexpr std::string kNewMainRuleName = "__main__";
    static constexpr std::string kEmptyRuleName = "__empty__";

public:
    GrammarInfo(const Grammar& grammar);

    const Grammar& GetGrammar() const;
    size_t GetMainRule() const;
    const std::string GetOldMainRuleName() const;
    const std::set<std::string>& GetUsedTokens() const;
    const std::set<std::string>& GetUsedRules() const;
    const std::set<Symbol>& GetUsedSymbols() const;
    const std::set<std::string>& GetUsedReturnTypes() const;
    const std::map<Symbol, std::set<std::string>>& GetFirstMap() const;
    const std::map<std::string, std::set<std::string>>& GetFollowMap() const;
    const std::set<std::string>& GetFollow(const std::string& name) const;

private:
    void Build();
    void UpdateMainRule();
    void AddUsedToken(const std::string& name);
    void AddUsedRule(const std::string& name);
    void AddUsedReturnTypeFor(const std::string& name);
    void AddUsedSymbol(const Symbol& symbol);
    void AddUsedHandlerName(const std::string& handler_name);
    void BuildUsedSets();
    void CheckUsedRules();
    void BuildFirstValues();
    void BuildFollowValues();
    bool MoveTokens(const std::set<std::string>& values, std::set<std::string>& result);

private:
    Grammar grammar_;
    std::string old_main_rule_name_;

    std::set<std::string> used_tokens_;
    std::set<std::string> used_rules_;
    std::set<Symbol> used_symbols_;
    std::set<std::string> used_return_types_;
    std::set<std::string> used_handler_names_;

    std::map<Symbol, std::set<std::string>> first_;
    std::map<std::string, std::set<std::string>> follow_;
};

}  // namespace grammar

std::ostream& operator<<(std::ostream& out, const grammar::GrammarInfo& info);
