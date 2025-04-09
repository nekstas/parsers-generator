#pragma once
#include <cstdint>

namespace pg {
enum class Identifier : size_t {
    Production,
    ProductionList,
    Rule,
    RuleHeader,
    RuleList,
    SequenceOfSymbols,
    Symbol,
    TakeSymbol,
};
}
