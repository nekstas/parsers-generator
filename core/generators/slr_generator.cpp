#include "slr_generator.h"

#include "../../utils/format_stream.h"
#include "../app/errors.h"

generators::SlrGenerator::SlrGenerator(const grammar::GrammarInfo& grammar_info)
    : grammar_info_(grammar_info), grammar_(grammar_info.GetGrammar()) {
    BuildStates();
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
        Visualize(out, item);
        out << "\n";
    }
}

generators::Lr0Item generators::SlrGenerator::GetMainItem() const {
    return {*grammar_.GetRulesFor(grammar_.GetMainRule()).begin(), 0};
}

bool generators::Lr0Item::operator<(const generators::Lr0Item& other) const {
    return std::tie(rule, pos) < std::tie(other.rule, other.pos);
}

void generators::SlrGenerator::Visualize(std::ostream& out, const generators::Lr0Item& item) const {
    const grammar::Rule& rule_object = grammar_.GetRule(item.rule);
    out << "<" << rule_object.name << ">" << " ::=";
    for (size_t i = 0; i < rule_object.sequence.size(); ++i) {
        out << " ";
        if (i == item.pos) {
            out << "·";
        }
        out << rule_object.sequence[i];
    }
    if (item.pos == rule_object.sequence.size()) {
        out << " ·";
    }
}

bool generators::SlrStates::HasState(const generators::SetOfLr0Items& items) const {
    return state_by_set_.contains(items);
}

void generators::SlrStates::AddState(const generators::SetOfLr0Items& items) {
    if (HasState(items)) {
        return;
    }

    size_t index = states_.size();
    states_.push_back(items);
    state_by_set_[items] = index;
    goto_.emplace_back();
}

size_t generators::SlrStates::GetCount() const {
    return states_.size();
}

const generators::SetOfLr0Items& generators::SlrStates::GetState(size_t index) const {
    return states_.at(index);
}

void generators::SlrStates::AddEdge(const generators::SetOfLr0Items& from,
                                    const generators::SetOfLr0Items& to,
                                    const grammar::Symbol& symbol) {
    size_t from_index = GetState(from);
    size_t to_index = GetState(to);
    goto_[from_index][symbol] = to_index;
}

size_t generators::SlrStates::GetState(const generators::SetOfLr0Items& items) const {
    return state_by_set_.at(items);
}

const std::map<grammar::Symbol, size_t>& generators::SlrStates::GetGoto(size_t index) const {
    return goto_.at(index);
}

void generators::SlrGenerator::BuildStates() {
    states_ = {};
    states_.AddState(Closure({GetMainItem()}));

    for (size_t i = 0; i < states_.GetCount(); ++i) {
        auto items = states_.GetState(i);
        for (const grammar::Symbol& symbol : grammar_info_.GetUsedSymbols()) {
            auto new_items = Goto(items, symbol);
            if (!new_items.empty()) {
                states_.AddState(new_items);
                states_.AddEdge(items, new_items, symbol);
            }
        }
    }
}

void generators::SlrGenerator::Visualize(std::ostream& out) const {
    out << "[SLR States]\n";
    for (size_t i = 0; i < states_.GetCount(); ++i) {
        out << "State [" << i << "]";
        for (const auto& [symbol, j] : states_.GetGoto(i)) {
            out << ", " << symbol << " -> " << j;
        }
        out << "\n";
        Visualize(out, states_.GetState(i));
        out << "\n";
    }
}

generators::LrTables generators::SlrGenerator::GenerateTables() {
    LrTables result(states_.GetCount());

    for (size_t i = 0; i < states_.GetCount(); ++i) {
        const auto& state = states_.GetState(i);
        const auto& edges = states_.GetGoto(i);

        for (const auto& [symbol, new_state] : edges) {
            if (symbol.type == grammar::Symbol::Type::Terminal) {
                result.AddShiftAction(i, symbol.value, edges.at(symbol), *this);
            } else {
                result.AddGoto(i, symbol.value, edges.at(symbol));
            }
        }

        for (const Lr0Item& item : state) {
            const grammar::Rule& rule = grammar_.GetRule(item.rule);

            if (item.pos != rule.sequence.size()) {
                continue;
            } else if (rule.name == grammar_.GetMainRule()) {
                result.AddAcceptAction(i, grammar::GrammarInfo::kEofTokenName, *this);
                continue;
            }

            for (const std::string& token : grammar_info_.GetFollow(rule.name)) {
                result.AddReduceAction(i, token, item.rule, *this);
            }
        }

        for (const grammar::Symbol& symbol : grammar_info_.GetUsedSymbols()) {
            if (symbol.value != grammar::GrammarInfo::kNewMainRuleName) {
                result.SetErrorIfNoAction(i, symbol, *this);
            }
        }
    }

    return result;
}

const generators::SlrStates& generators::SlrGenerator::GetStates() const {
    return states_;
}

generators::LrTables::LrTables(size_t size) : action_table(size), goto_table(size) {
}

void generators::LrTables::AddAction(size_t state, const std::string& token,
                                     const generators::LrAction& action,
                                     const SlrGenerator& generator) {
    if (action_table.at(state).contains(token) && action != action_table.at(state).at(token)) {
        const auto& another_action = action_table.at(state).at(token);
        std::stringstream error_msg;
        error_msg << "There is a " << action.TypeToString() << "/" << another_action.TypeToString()
                  << " conflict.\n";
        error_msg << "This grammar cannot be recognized by SLR(1)-parser.\nTry "
                     "to rewrite a grammar for your language.\n";
        error_msg << "Additional info:\n";
        error_msg << "Token: " << token << ", SLR(0)-item (index=" << state << "):\n";
        generator.Visualize(error_msg, generator.GetStates().GetState(state));

        throw errors::ApplicationError{error_msg.str()};
    }
    action_table[state][token] = action;
}

void generators::LrTables::AddShiftAction(size_t state, const std::string& token, size_t new_state,
                                          const SlrGenerator& generator) {
    AddAction(state, token, {LrAction::Type::SHIFT, new_state}, generator);
}

void generators::LrTables::AddReduceAction(size_t state, const std::string& token, size_t rule,
                                           const SlrGenerator& generator) {
    AddAction(state, token, {LrAction::Type::REDUCE, rule}, generator);
}

void generators::LrTables::AddAcceptAction(size_t state, const std::string& token,
                                           const SlrGenerator& generator) {
    AddAction(state, token, {LrAction::Type::ACCEPT, 0}, generator);
}

void generators::LrTables::AddGoto(size_t state, const std::string& symbol, size_t new_state) {
    if (goto_table[state].contains(symbol)) {
        throw std::logic_error{"There is a goto conflict. But it is impossible."};
    }
    goto_table[state][symbol] = new_state;
}

void generators::LrTables::SetErrorIfNoAction(size_t state, const grammar::Symbol& symbol,
                                              const SlrGenerator& generator) {
    if (symbol.type == grammar::Symbol::Type::Terminal) {
        if (!action_table[state].contains(symbol.value)) {
            AddErrorAction(state, symbol.value, generator);
        }
    } else {
        if (!goto_table[state].contains(symbol.value)) {
            AddGoto(state, symbol.value, -1);
        }
    }
}

void generators::LrTables::AddErrorAction(size_t state, const std::string& token,
                                          const SlrGenerator& generator) {
    AddAction(state, token, {LrAction::Type::ERROR, 0}, generator);
}
