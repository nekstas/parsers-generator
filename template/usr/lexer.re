#include "lexer.h"

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
        "(" { return TokenType::OpenBracket; }
        ")" { return TokenType::CloseBracket; }

    */
    // NOLINTEND

    return TokenType::Unknown;
}
