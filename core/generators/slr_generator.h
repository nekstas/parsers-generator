#pragma once

#include <cstdint>
#include <set>

#include "../grammar/grammar_info.h"

namespace generators {

struct Lr0Item {
    size_t rule;
    size_t pos;

    bool operator<(const Lr0Item& other) const;
};

using SetOfLr0Items = std::set<Lr0Item>;

class SlrStates {
public:
    size_t GetCount() const;
    bool HasState(const SetOfLr0Items& items) const;
    void AddState(const SetOfLr0Items& items);
    const SetOfLr0Items& GetState(size_t index) const;
    size_t GetState(const SetOfLr0Items& items) const;
    const std::map<grammar::Symbol, size_t>& GetGoto(size_t index) const;

    void AddEdge(const generators::SetOfLr0Items& from, const generators::SetOfLr0Items& to,
                 const grammar::Symbol& symbol);

private:
    std::vector<SetOfLr0Items> states_;
    std::map<SetOfLr0Items, size_t> state_by_set_;
    std::vector<std::map<grammar::Symbol, size_t>> goto_;
};

class SlrGenerator {
public:
    SlrGenerator(const grammar::GrammarInfo& grammar_info);

    void Visualize(std::ostream& out, const Lr0Item& item) const;
    void Visualize(std::ostream& out, const SetOfLr0Items& items) const;
    void Visualize(std::ostream& out) const;

private:
    Lr0Item GetMainItem() const;
    void BuildStates();

    SetOfLr0Items Closure(SetOfLr0Items items) const;
    SetOfLr0Items Goto(const SetOfLr0Items& items, const grammar::Symbol& symbol) const;

private:
    grammar::GrammarInfo grammar_info_;
    const grammar::Grammar& grammar_;
    SlrStates states_;
};

}  // namespace generators