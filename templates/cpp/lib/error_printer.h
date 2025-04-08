#pragma once

#include "errors.h"
#include "tokenizer.h"

namespace pg {

class ErrorPrinter {
public:
    static void SourceCodeError(const std::string& message, const std::string& line_content,
                                size_t line, size_t pos, size_t len);

    static void Print(const TokenizerError& error);
    static void Print(const ParserError& error);
};

}  // namespace pg
