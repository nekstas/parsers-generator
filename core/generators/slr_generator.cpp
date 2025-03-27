#include "slr_generator.h"

generators::SlrGenerator::SlrGenerator(const grammar::GrammarInfo& grammar_info)
    : grammar_info_(grammar_info), grammar_(grammar_info.GetGrammar()) {

    std::cerr << "\n";
    auto x = Closure({GetMainItem()});
    Visualize(std::cerr, x);
    std::cerr << "\n";
    auto y = Goto(x, {grammar::Symbol::Type::NonTerminal, "E"});
    Visualize(std::cerr, y);
    std::cerr << "\n";
    auto z = Goto(y, {grammar::Symbol::Type::Terminal, "Plus"});
    Visualize(std::cerr, z);
    std::cerr << "\n";
}

generators::SetOfLr0Items generators::SlrGenerator::Closure(generators::SetOfLr0Items items) const {
    while (true) {
        SetOfLr0Items new_items;

        for (const Lr0Item& item : items) {
            const grammar::Rule& rule = grammar_.GetRule(item.rule);

            if (item.pos >= rule.sequence.size() ||
                rule.sequence[item.pos].type != grammar::Symbol::Type::NonTerminal) {
                continue;
            }

            for (size_t id : grammar_.GetRulesFor(rule.sequence[item.pos].value)) {
                if (!items.contains({id, 0})) {
                    new_items.insert({id, 0});
                }
            }
        }

        if (new_items.empty()) {
            break;
        }
        items.insert(new_items.begin(), new_items.end());
    }

    return items;
}

generators::SetOfLr0Items generators::SlrGenerator::Goto(const generators::SetOfLr0Items& items,
                                                         const grammar::Symbol& symbol) const {
    SetOfLr0Items new_items;

    for (const Lr0Item& item : items) {
        const grammar::Rule& rule = grammar_.GetRule(item.rule);

        if (item.pos >= rule.sequence.size() || rule.sequence[item.pos] != symbol) {
            continue;
        }

        new_items.insert({item.rule, item.pos + 1});
    }

    return Closure(new_items);
}

void generators::SlrGenerator::Visualize(std::ostream& out,
                                         const generators::SetOfLr0Items& items) const {
    const auto& grammar = grammar_info_.GetGrammar();
    for (const generators::Lr0Item& item : items) {
        item.Visualize(out, grammar);
        out << "\n";
    }
}

generators::Lr0Item generators::SlrGenerator::GetMainItem() const {
    return {*grammar_.GetRulesFor(grammar_.GetMainRule()).begin(), 0};
}

bool generators::Lr0Item::operator<(const generators::Lr0Item& other) const {
    return std::tie(rule, pos) < std::tie(other.rule, other.pos);
}

void generators::Lr0Item::Visualize(std::ostream& out, const grammar::Grammar& grammar) const {
    const grammar::Rule& rule_object = grammar.GetRule(rule);
    out << "<" << rule_object.name << ">" << " ::=";
    for (size_t i = 0; i < rule_object.sequence.size(); ++i) {
        out << " ";
        if (i == pos) {
            out << "·";
        }
        out << rule_object.sequence[i];
    }
    if (pos == rule_object.sequence.size()) {
        out << " ·";
    }
}
