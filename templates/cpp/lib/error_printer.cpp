#include "error_printer.h"

#include <iostream>

void pg::ErrorPrinter::UnexpectedToken(const pg::Tokenizer::Result& input, size_t error_index) {
    const Token& token = input.tokens[error_index];
    std::cerr << "An error has occurred: unexpected token ";
    std::cerr << "on line " << (token.line + 1) << ", character " << (token.pos + 1) << "\n";
    std::cerr << "| " << input.lines[token.line] << "\n";
    std::cerr << "| " << std::string(token.pos, ' ') << std::string(token.value.size(), '^')
              << "\n";
}
