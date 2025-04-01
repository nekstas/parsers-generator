#include "application.h"

#include "../code/cpp_generator.h"
#include "../generators/slr_generator.h"
#include "../grammar/grammar.h"
#include "../grammar/grammar_info.h"

int32_t Application::Run(size_t argc, char** argv) {
    try {
        if (argc != kArgsCount) {
            PrintHelpMessage();
        } else {
            GenerateParser(argv[1], argv[2]);
        }
    } catch (std::runtime_error error) {
        // TODO: handle an error
        return 1;
    } catch (...) {
        // TODO: handle an error
        return 2;
    }
    return 0;
}

grammar::Symbol MakeT(const std::string& name) {
    return {grammar::Symbol::Type::Terminal, name};
}

grammar::Symbol MakeNT(const std::string& name) {
    return {grammar::Symbol::Type::NonTerminal, name};
}

void Application::GenerateParser(const std::string& grammar_file, const std::string& output_path) {
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
}

void Application::PrintHelpMessage() {
}
