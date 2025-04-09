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
        "->" { return TokenType::ArrowRight; }
        "]" { return TokenType::CloseSq; }
        ">" { return TokenType::CloseTr; }
        ":" { return TokenType::Colon; }
        "@" { return TokenType::At; }
        "$" { return TokenType::Dollar; }
        [a-zA-Z][a-zA-Z0-9]* { return TokenType::Name; }
        "[" { return TokenType::OpenSq; }
        "<" { return TokenType::OpenTr; }
        [\t\n\r ] { return TokenType::Skip; }
        "#".+ { return TokenType::Skip; }

        // Your rules here...

    */
    // NOLINTEND

    return TokenType::Unknown;
}