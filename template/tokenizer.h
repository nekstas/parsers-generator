#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "lexer.h"

namespace pg {

class Tokenizer {
private:
    struct Context {
        const char* prev = nullptr;
        const char* current = nullptr;
        const char* end = nullptr;

        const char* line_begin = nullptr;
        size_t line = 0;
        size_t pos = 0;

        Context() = default;
        explicit Context(const std::string& code)
            : prev(code.c_str()), current(prev), end(prev + code.size()), line_begin(prev) {
        }
    };

public:
    struct Result {
        std::vector<lexer::Token> tokens;
        std::vector<std::string> lines;
    };

public:
    Result Tokenize(const std::string& code);

    void Init(const std::string& code);
    bool IsEnd();
    void ProcessNewToken();
    void ProcessPosition();
    void AddEofToken();

private:
    lexer::Lexer lexer_;
    Context context_;
    Result result_;
};

}  // namespace pg
