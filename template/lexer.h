#pragma once

#include <cstdint>
#include <string>

namespace lexer {

enum class TokenType : size_t {
    Eof,      // End Of File,
    Skip,     // Don't use a token in parser
    Unknown,  // Unknown token, throw an error
    // Add more tokens here...
};

class Token {
public:
    Token(TokenType type, std::string value, size_t line, size_t pos);

    TokenType GetTokenType() const;
    const std::string& GetValue() const;
    const size_t GetLine() const;
    const size_t GetPos() const;

private:
    TokenType type_;
    std::string value_;
    size_t line_;
    size_t pos_;
};

class Lexer {
public:
    TokenType GetTokenType(const char*& current, const char* end);

private:
    // You can add some properties here if it needs
};

}  // namespace lexer
