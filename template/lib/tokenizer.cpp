#include "tokenizer.h"

pg::Tokenizer::Result pg::Tokenizer::Tokenize(const std::string& code) {
    Init(code);

    while (!IsEnd()) {
        ProcessNewToken();
        ProcessNewLines();
    }

    ProcessEnd();
    return result_;
}

void pg::Tokenizer::Init(const std::string& code) {
    lexer_ = pg::Lexer{};
    context_ = Context{code};
    result_ = Result{};
}

bool pg::Tokenizer::IsEnd() {
    return context_.current == context_.end;
}

void pg::Tokenizer::ProcessNewToken() {
    pg::TokenType type = lexer_.GetTokenType(context_.current, context_.end);
    if (type == pg::TokenType::Unknown) {
        // TODO: throw an error
        throw std::runtime_error{"Unknown Token."};
    } else if (type == pg::TokenType::Eof) {
        // TODO: think, what we should do here
        throw std::runtime_error{"Unexpected Eof."};
    } else if (type != pg::TokenType::Skip) {
        result_.tokens.emplace_back(type, std::string(context_.prev, context_.current),
                                    GetCurrentLine(), GetCurrentPos());
    }
}

void pg::Tokenizer::ProcessNewLines() {
    for (; context_.prev != context_.current; ++context_.prev) {
        if (*context_.prev == '\n') {
            AddLineToResult();
        }
    }
}

void pg::Tokenizer::AddEofToken() {
    result_.tokens.emplace_back(pg::TokenType::Eof, "", GetCurrentLine(), GetCurrentPos());
}

void pg::Tokenizer::ProcessEnd() {
    AddEofToken();
    AddLineToResult();
}

void pg::Tokenizer::AddLineToResult() {
    result_.lines.emplace_back(context_.line_begin, context_.prev);
    context_.line_begin = context_.prev + 1;
}

size_t pg::Tokenizer::GetCurrentLine() {
    return result_.lines.size();
}

size_t pg::Tokenizer::GetCurrentPos() {
    return context_.prev - context_.line_begin;
}
