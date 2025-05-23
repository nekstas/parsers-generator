#pragma once

#include "../data/ast_builder.h"
#include "../usr/lexer.h"
#include "lr_data.h"
#include "tokenizer.h"

namespace pg {

class LrParser {
public:
    LrParser(const LrData& tables);

    void Parse(const Tokenizer::Result& input, AstBuilder& ast_builder) const;
    void Parse(const std::string& code, AstBuilder& ast_builder) const;

    static LrParser Create();

private:
    LrData data_;
};

}  // namespace pg