#include "tokenizer.h"

pg::Tokenizer::Result pg::Tokenizer::Tokenize(const std::string& code) {
    Init(code);

    while (!IsEnd()) {
        ProcessNewToken();
        ProcessPosition();
    }

    AddEofToken();
    return result_;
}

void pg::Tokenizer::Init(const std::string& code) {
    lexer_ = lexer::Lexer{};
    context_ = Context{code};
    result_ = Result{};
}

bool pg::Tokenizer::IsEnd() {
    return context_.current == context_.end;
}

void pg::Tokenizer::ProcessNewToken() {
    lexer::TokenType type = lexer_.GetTokenType(context_.current, context_.end);
    if (type == lexer::TokenType::Unknown) {
        // TODO: throw an error
    } else if (type == lexer::TokenType::Eof) {
        // TODO: think, what we should do here
    } else if (type != lexer::TokenType::Skip) {
        result_.tokens.emplace_back(type, std::string(context_.prev, context_.current),
                                    context_.line, context_.pos);
    }
}

void pg::Tokenizer::ProcessPosition() {
    for (; context_.prev != context_.current; ++context_.prev) {
        if (*context_.prev == '\n') {
            result_.lines.emplace_back(context_.line_begin, context_.prev);
            context_.line_begin = context_.prev + 1;
            ++context_.line;
            context_.pos = 0;
        } else {
            ++context_.pos;
        }
    }
}

void pg::Tokenizer::AddEofToken() {
    result_.tokens.emplace_back(lexer::TokenType::Eof, "", context_.line, context_.pos);
}
