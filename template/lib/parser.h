#pragma once

#include "../lexer.h"
#include "lr_data.h"

namespace pg {

class LrParser {
public:
    LrParser(const LrData& tables);

    void Parse(const std::vector<Token>& tokens);

private:
    LrData data_;
};

}  // namespace pg
