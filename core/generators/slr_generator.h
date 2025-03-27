#pragma once

#include <cstdint>
#include <set>

#include "../grammar/grammar_info.h"

namespace generators {

struct Lr0Item {
    size_t rule;
    size_t pos;

    bool operator<(const Lr0Item& other) const;

    void Visualize(std::ostream& out, const grammar::Grammar& grammar) const;
};

using SetOfLr0Items = std::set<Lr0Item>;

class SlrGenerator {
public:
    SlrGenerator(const grammar::GrammarInfo& grammar_info);

private:
    Lr0Item GetMainItem() const;
    SetOfLr0Items Closure(SetOfLr0Items items) const;
    SetOfLr0Items Goto(const SetOfLr0Items& items, const grammar::Symbol& symbol) const;
    void Visualize(std::ostream& out, const SetOfLr0Items& items) const;

private:
    grammar::GrammarInfo grammar_info_;
    const grammar::Grammar& grammar_;
};

}  // namespace generators