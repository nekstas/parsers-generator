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

    */
    // NOLINTEND

    return TokenType::Unknown;
}
