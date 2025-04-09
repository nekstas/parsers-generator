#pragma once

#include "../grammar/grammar.h"
#include "data/grammar.h"

class GrammarParser {
public:
    grammar::Grammar Parse(const std::string& path) const;
};