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
    /*
     [F: Number]
     * $Number -> Number
     * OpenBracket $<E> CloseBracket -> BracketExpression

     [T: Number]
     * $<T> Star $<F> -> MultiplyExpression
     * $<F> -> F2T

     [E: Number]
     * $<E> Plus $<T> -> SumExpression
     * $<T> -> T2E
     * */
    grammar::Grammar grammar;
    grammar.AddRule({"F", "Number", {MakeT("Number")}, {true}});
    grammar.AddRule({"F",
                     "BracketExpression",
                     {MakeT("OpenBracket"), MakeNT("E"), MakeT("CloseBracket")},
                     {false, true, false}});
    grammar.AddRule({"T",
                     "MultiplyExpression",
                     {MakeNT("T"), MakeT("Star"), MakeNT("F")},
                     {true, false, true}});
    grammar.AddRule({"T", "F2T", {MakeNT("F")}, {true}});
    grammar.AddRule(
        {"E", "SumExpression", {MakeNT("E"), MakeT("Plus"), MakeNT("T")}, {true, false, true}});
    grammar.AddRule({"E", "T2E", {MakeNT("T")}, {true}});
    grammar.SetReturnType("T", "Number");
    grammar.SetReturnType("F", "Number");
    grammar.SetReturnType("E", "Number");
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
