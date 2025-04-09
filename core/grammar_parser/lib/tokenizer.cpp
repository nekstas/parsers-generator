#include "tokenizer.h"

#include "errors.h"

pg::Tokenizer::Result pg::Tokenizer::Tokenize(const std::string& code) {
    Init(code);
    SplitIntoLines();

    while (!IsEnd()) {
        ProcessNewToken();
        ProcessNewLines();
    }

    AddEofToken();
    return result_;
}

void pg::Tokenizer::Init(const std::string& code) {
    lexer_ = pg::Lexer{};
    context_ = Context{code};
    result_ = Result{};
}

void pg::Tokenizer::SplitIntoLines() {
    const char* prev = context_.current;
    for (const char* current = context_.current; current != context_.end; ++current) {
        if (*current == '\n') {
            result_.lines.emplace_back(prev, current);
            prev = current + 1;
        }
    }
    result_.lines.emplace_back(prev, context_.end);
}

bool pg::Tokenizer::IsEnd() {
    return context_.current == context_.end;
}

void pg::Tokenizer::ProcessNewToken() {
    pg::TokenType type = lexer_.GetTokenType(context_.current, context_.end);
    if (type == pg::TokenType::Unknown) {
        throw pg::TokenizerError("unknown token", result_.lines, GetCurrentLine(), GetCurrentPos());
    } else if (type == pg::TokenType::Eof) {
        throw std::logic_error{"Lexer should not return Eof token."};
    } else if (type != pg::TokenType::Skip) {
        result_.tokens.emplace_back(type, std::string(context_.prev, context_.current),
                                    GetCurrentLine(), GetCurrentPos());
    }
}

void pg::Tokenizer::ProcessNewLines() {
    for (; context_.prev != context_.current; ++context_.prev) {
        if (*context_.prev == '\n') {
            context_.line_begin = context_.prev + 1;
            ++context_.line_number;
        }
    }
}

void pg::Tokenizer::AddEofToken() {
    result_.tokens.emplace_back(pg::TokenType::Eof, "", GetCurrentLine(), GetCurrentPos());
}

size_t pg::Tokenizer::GetCurrentLine() {
    return context_.line_number;
}

size_t pg::Tokenizer::GetCurrentPos() {
    return context_.prev - context_.line_begin;
}#include "tokenizer.h"

#include "errors.h"

pg::Tokenizer::Result pg::Tokenizer::Tokenize(const std::string& code) {
    Init(code);
    SplitIntoLines();

    while (!IsEnd()) {
        ProcessNewToken();
        ProcessNewLines();
    }

    AddEofToken();
    return result_;
}

void pg::Tokenizer::Init(const std::string& code) {
    lexer_ = pg::Lexer{};
    context_ = Context{code};
    result_ = Result{};
}

void pg::Tokenizer::SplitIntoLines() {
    const char* prev = context_.current;
    for (const char* current = context_.current; current != context_.end; ++current) {
        if (*current == '\n') {
            result_.lines.emplace_back(prev, current);
            prev = current + 1;
        }
    }
    result_.lines.emplace_back(prev, context_.end);
}

bool pg::Tokenizer::IsEnd() {
    return context_.current == context_.end;
}

void pg::Tokenizer::ProcessNewToken() {
    pg::TokenType type = lexer_.GetTokenType(context_.current, context_.end);
    if (type == pg::TokenType::Unknown) {
        throw pg::TokenizerError("unknown token", result_.lines, GetCurrentLine(), GetCurrentPos());
    } else if (type == pg::TokenType::Eof) {
        throw std::logic_error{"Lexer should not return Eof token."};
    } else if (type != pg::TokenType::Skip) {
        result_.tokens.emplace_back(type, std::string(context_.prev, context_.current),
                                    GetCurrentLine(), GetCurrentPos());
    }
}

void pg::Tokenizer::ProcessNewLines() {
    for (; context_.prev != context_.current; ++context_.prev) {
        if (*context_.prev == '\n') {
            context_.line_begin = context_.prev + 1;
            ++context_.line_number;
        }
    }
}

void pg::Tokenizer::AddEofToken() {
    result_.tokens.emplace_back(pg::TokenType::Eof, "", GetCurrentLine(), GetCurrentPos());
}

size_t pg::Tokenizer::GetCurrentLine() {
    return context_.line_number;
}

size_t pg::Tokenizer::GetCurrentPos() {
    return context_.prev - context_.line_begin;
}