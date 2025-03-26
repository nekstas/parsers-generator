#include <iostream>

#include "template/tokenizer.h"

int main() {
    std::string code;
    pg::Tokenizer::Result result = pg::Tokenizer().Tokenize(code);

    std::cerr << "Tokens:\n";
    for (auto token : result.tokens) {
        std::cerr << "type=" << static_cast<size_t>(token.GetTokenType());
        std::cerr << "|value=" << token.GetValue();
        std::cerr << "|line=" << token.GetLine() + 1;
        std::cerr << "|pos=" << token.GetPos() + 1;
        std::cerr << "\n";
    }

    std::cerr << "\n";

    std::cerr << "Lines:\n";
    for (size_t i = 0; i < result.lines.size(); ++i) {
        std::cerr << "[" << i << "]: |" << result.lines[i] << "|\n";
    }

    return 0;
}
