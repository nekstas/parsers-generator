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

struct LrAction {
    enum class Type { SHIFT, REDUCE, ACCEPT, ERROR };
    Type type;
    size_t index;

    bool operator==(const LrAction& other) const {
        return std::tie(type, index) == std::tie(other.type, other.index);
    }

    std::string TypeToString() const {
        switch (type) {
            case Type::SHIFT:
                return "SHIFT";
            case Type::REDUCE:
                return "REDUCE";
            case Type::ACCEPT:
                return "ACCEPT";
            case Type::ERROR:
                return "ERROR";
        }
        return "UNKNOWN";
    }
};

using LrActionTable = std::vector<std::map<std::string, LrAction>>;
using LrGotoTable = std::vector<std::map<std::string, size_t>>;

class SlrGenerator;

struct LrTables {
    LrActionTable action_table;
    LrGotoTable goto_table;

    LrTables(size_t size);

    void AddAction(size_t state, const std::string& token, const LrAction& action,
                   const SlrGenerator& generator);
    void AddShiftAction(size_t state, const std::string& token, size_t new_state,
                        const SlrGenerator& generator);
    void AddReduceAction(size_t state, const std::string& token, size_t rule,
                         const SlrGenerator& generator);
    void AddAcceptAction(size_t state, const std::string& token, const SlrGenerator& generator);
    void AddErrorAction(size_t state, const std::string& token, const SlrGenerator& generator);

    void AddGoto(size_t state, const std::string& symbol, size_t new_state);
    void SetErrorIfNoAction(size_t state, const grammar::Symbol& symbol,
                            const SlrGenerator& generator);
};

class SlrGenerator {
public:
    SlrGenerator(const grammar::GrammarInfo& grammar_info);

    LrTables GenerateTables();

    const SlrStates& GetStates() const;

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

std::ostream& operator<<(std::ostream& out, const generators::LrAction& action);
