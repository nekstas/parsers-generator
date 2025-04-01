#include <iostream>

#include "lib/parser.h"
#include "lib/tokenizer.h"
#include "usr/calculator.h"

int main() {
    std::string code = "1 + 2 * 3 + 10 * 10 \n\n           23445568";
    pg::Tokenizer::Result result = pg::Tokenizer().Tokenize(code);

    std::cerr << "Tokens:\n";
    for (auto token : result.tokens) {
        std::cerr << "[type=" << static_cast<size_t>(token.type);
        std::cerr << "|value=" << token.value;
        std::cerr << "|line=" << token.line + 1;
        std::cerr << "|pos=" << token.pos + 1;
        std::cerr << "]\n";
    }

    std::cerr << "\n";

    std::cerr << "Lines:\n";
    for (size_t i = 0; i < result.lines.size(); ++i) {
        std::cerr << "[" << i << "]: |" << result.lines[i] << "|\n";
    }
    std::cerr << "\n";

    pg::LrParser parser = pg::LrParser::Create();
    Calculator calculator;
    parser.Parse(result, calculator);

    return 0;
}
