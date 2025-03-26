#include "lexer.h"

lexer::Token::Token(lexer::TokenType type, std::string value, size_t line, size_t pos)
    : type_(type), value_(value), line_(line), pos_(pos) {
}

lexer::TokenType lexer::Token::GetTokenType() const {
    return type_;
}

const std::string& lexer::Token::GetValue() const {
    return value_;
}

const size_t lexer::Token::GetLine() const {
    return line_;
}

const size_t lexer::Token::GetPos() const {
    return pos_;
}

lexer::TokenType lexer::Lexer::GetTokenType(const char*& str_ptr, const char* end) {
    const char* marker = nullptr;

    // NOLINTBEGIN
    /*!re2c
        re2c:define:YYCTYPE = "unsigned char";
        re2c:define:YYCURSOR = "str_ptr";
        re2c:define:YYLIMIT = "end";
        re2c:define:YYMARKER = "marker";
        re2c:yyfill:enable = 0;

        * { return TokenType::Unknown; }

        // Your rules here...
    */
    // NOLINTEND

    return TokenType::Unknown;
}
