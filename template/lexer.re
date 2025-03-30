#include "lexer.h"

pg::Token::Token(pg::TokenType type, std::string value, size_t line, size_t pos)
    : type_(type), value_(value), line_(line), pos_(pos) {
}

pg::TokenType pg::Token::GetTokenType() const {
    return type_;
}

const std::string& pg::Token::GetValue() const {
    return value_;
}

const size_t pg::Token::GetLine() const {
    return line_;
}

const size_t pg::Token::GetPos() const {
    return pos_;
}

pg::TokenType pg::Lexer::GetTokenType(const char*& str_ptr, const char* end) {
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
        [1-9][0-9]* { return TokenType::Number; }
        [ \n\s\t] { return TokenType::Skip; }

        "+" { return TokenType::Plus; }
        "*" { return TokenType::Star; }

    */
    // NOLINTEND

    return TokenType::Unknown;
}
