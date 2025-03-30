#include <iostream>

#include "core/generators/slr_generator.h"
#include "core/grammar/grammar.h"
#include "core/grammar/grammar_info.h"
#include "template/tokenizer.h"

grammar::Symbol MakeT(const std::string& name) {
    return {grammar::Symbol::Type::Terminal, name};
}

grammar::Symbol MakeNT(const std::string& name) {
    return {grammar::Symbol::Type::NonTerminal, name};
}

int main() {
    std::string code = "1 + 2 * 3";
    pg::Tokenizer::Result result = pg::Tokenizer().Tokenize(code);

    std::cerr << "Tokens:\n";
    for (auto token : result.tokens) {
        std::cerr << "[type=" << static_cast<size_t>(token.GetTokenType());
        std::cerr << "|value=" << token.GetValue();
        std::cerr << "|line=" << token.GetLine() + 1;
        std::cerr << "|pos=" << token.GetPos() + 1;
        std::cerr << "]\n";
    }

    std::cerr << "\n";

    std::cerr << "Lines:\n";
    for (size_t i = 0; i < result.lines.size(); ++i) {
        std::cerr << "[" << i << "]: |" << result.lines[i] << "|\n";
    }
    std::cerr << "\n";

    grammar::Grammar grammar;
    grammar.AddRule({"F", {MakeT("Number")}});
    grammar.AddRule({"F", {MakeT("OpenBracket"), MakeNT("E"), MakeT("CloseBracket")}});
    grammar.AddRule({"T", {MakeNT("T"), MakeT("Star"), MakeNT("F")}});
    grammar.AddRule({"T", {MakeNT("F")}});
    grammar.AddRule({"E", {MakeNT("E"), MakeT("Plus"), MakeNT("T")}});
    grammar.AddRule({"E", {MakeNT("T")}});
    grammar.SetMainRule("E");

    grammar::GrammarInfo grammar_info(grammar);
    std::cerr << grammar_info << "\n";

    generators::SlrGenerator generator(grammar_info);
    generator.Visualize(std::cerr);

    generators::LrTables tables = generator.GenerateTables();

    return 0;
}
