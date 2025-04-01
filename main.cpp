#include <iostream>

#include "core/code/cpp_generator.h"
#include "core/generators/slr_generator.h"
#include "core/grammar/grammar.h"
#include "core/grammar/grammar_info.h"
// #include "templates/cpp/lib/parser.h"
// #include "templates/cpp/lib/tokenizer.h"

grammar::Symbol MakeT(const std::string& name) {
    return {grammar::Symbol::Type::Terminal, name};
}

grammar::Symbol MakeNT(const std::string& name) {
    return {grammar::Symbol::Type::NonTerminal, name};
}

int main() {
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
    code::CppGenerator code_gen(grammar_info, tables);
    code_gen.Generate("../templates/cpp");

    return 0;
}
