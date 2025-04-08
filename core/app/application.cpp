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

    grammar.SetReturnType("Symbol", "Symbol");
    grammar.AddRule({"Symbol", "Terminal", {MakeT("Name")}, {true}});
    grammar.AddRule({"Symbol",
                     "NonTerminal",
                     {MakeT("OpenTr"), MakeT("Name"), MakeT("CloseTr")},
                     {false, true, false}});

    grammar.SetReturnType("TakeSymbol", "TakeSymbol");
    grammar.AddRule({"TakeSymbol", "DontTakeSymbol", {MakeNT("Symbol")}, {true}});
    grammar.AddRule(
        {"TakeSymbol", "TakeSymbol", {MakeT("Dollar"), MakeNT("Symbol")}, {false, true}});

    grammar.SetReturnType("SequenceOfSymbols", "SequenceOfSymbols");
    grammar.AddRule(
        {"SequenceOfSymbols", "SequenceOfSymbolsBegin", {MakeNT("TakeSymbol")}, {true}});
    grammar.AddRule({"SequenceOfSymbols",
                     "SequenceOfSymbolsContinue",
                     {MakeNT("SequenceOfSymbols"), MakeNT("TakeSymbol")},
                     {true, true}});

    grammar.SetReturnType("Production", "Production");
    grammar.AddRule({"Production",
                     "Production",
                     {MakeNT("SequenceOfSymbols"), MakeT("ArrowRight"), MakeT("Name")},
                     {true, false, true}});

    grammar.SetReturnType("ProductionList", "ProductionList");
    grammar.AddRule({"ProductionList", "ProductionListBegin", {MakeNT("Production")}, {true}});
    grammar.AddRule({"ProductionList",
                     "ProductionListContinue",
                     {MakeNT("ProductionList"), MakeNT("Production")},
                     {true, true}});

    grammar.SetReturnType("RuleHeader", "RuleHeader");
    grammar.AddRule({"RuleHeader",
                     "CommonRuleHeader",
                     {MakeT("OpenSq"), MakeT("Name"), MakeT("CloseSq")},
                     {false, true, false}});
    grammar.AddRule(
        {"RuleHeader",
         "TypedRuleHeader",
         {MakeT("OpenSq"), MakeT("Name"), MakeT("Colon"), MakeT("Name"), MakeT("CloseSq")},
         {false, true, false, true, false}});

    grammar.SetReturnType("Rule", "Rule");
    grammar.AddRule(
        {"Rule", "CommonRule", {MakeNT("RuleHeader"), MakeNT("ProductionList")}, {true, true}});
    grammar.AddRule({"Rule",
                     "MainRule",
                     {MakeT("Dog"), MakeNT("RuleHeader"), MakeNT("ProductionList")},
                     {true, true}});

    grammar.SetReturnType("RuleList", "RuleList");
    grammar.AddRule({"RuleList", "RuleListBegin", {MakeNT("Rule")}, {true}});
    grammar.AddRule(
        {"RuleList", "RuleListContinue", {MakeNT("RuleList"), MakeNT("Rule")}, {true, true}});

    grammar.SetReturnType("Grammar", "Grammar");
    grammar.AddRule({"Grammar", "Grammar", {MakeNT("RuleList")}, {true}});
    grammar.SetMainRule("Grammar");

    grammar::GrammarInfo grammar_info(grammar);
    std::cerr << grammar_info << "\n";

    generators::SlrGenerator generator(grammar_info);
    generator.Visualize(std::cerr);

    generators::LrTables tables = generator.GenerateTables();
    code::CppGenerator code_gen(grammar_info, tables);
    code_gen.Create(output_path);
}

void Application::PrintHelpMessage() {
}
