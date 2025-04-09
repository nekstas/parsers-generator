#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "../usr/lexer.h"
#include "token.h"

namespace pg {

class Tokenizer {
private:
    struct Context {
        const char* prev = nullptr;
        const char* current = nullptr;
        const char* end = nullptr;
        const char* line_begin = nullptr;
        size_t line_number = 0;

        Context() = default;
        explicit Context(const std::string& code)
            : prev(code.c_str()),
              current(prev),
              end(prev + code.size()),
              line_begin(prev),
              line_number(0) {
        }
    };

public:
    struct Result {
        std::vector<pg::Token> tokens;
        std::vector<std::string> lines;
    };

public:
    Result Tokenize(const std::string& code);

private:
    void Init(const std::string& code);
    void SplitIntoLines();
    bool IsEnd();
    void ProcessNewToken();
    void ProcessNewLines();
    void AddEofToken();
    size_t GetCurrentLine();
    size_t GetCurrentPos();

private:
    pg::Lexer lexer_;
    Context context_;
    Result result_;
};

}  // namespace pg