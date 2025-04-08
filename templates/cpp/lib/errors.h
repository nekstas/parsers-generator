#pragma once

#include <stdexcept>
#include <string>
#include <vector>

#include "tokenizer.h"

namespace pg {

class TokenizerError : public std::exception {
public:
    TokenizerError(const std::string& message, const std::vector<std::string>& lines, size_t line,
                   size_t pos)
        : message(message), line_content(lines[line]), line(line), pos(pos) {
    }

    std::string message;
    std::string line_content;
    size_t line;
    size_t pos;
};

class ParserError : public std::exception {
public:
    ParserError(const std::string& message, const pg::Tokenizer::Result& input, size_t error_index)
        : message(message) {
        const Token& token = input.tokens[error_index];
        line_content = input.lines[token.line];
        std::tie(line, pos, len) = std::make_tuple(token.line, token.pos, token.value.size());
    }

    std::string message;
    std::string line_content;
    size_t line;
    size_t pos;
    size_t len;
};

}  // namespace pg
