#include "error_printer.h"

#include <iostream>

void pg::ErrorPrinter::SourceCodeError(const std::string& message, const std::string& line_content,
                                       size_t line, size_t pos, size_t len) {
    std::cerr << "An error has occurred: " << message;
    std::cerr << " on line " << (line + 1) << ", character " << (pos + 1) << "\n";
    std::cerr << "| " << line_content << "\n";
    std::cerr << "| " << std::string(pos, ' ') << std::string(len, '^') << "\n";
}

void pg::ErrorPrinter::Print(const pg::TokenizerError& error) {
    SourceCodeError(error.message, error.line_content, error.line, error.pos, 1);
}

void pg::ErrorPrinter::Print(const pg::ParserError& error) {
    SourceCodeError(error.message, error.line_content, error.line, error.pos, error.len);
}