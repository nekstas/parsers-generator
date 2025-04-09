#include "application.h"

#include "../code/cpp_generator.h"
#include "../grammar_parser/grammar_parser.h"
#include "errors.h"

int32_t Application::Run(size_t argc, char** argv) {
    try {
        if (argc != kRequiredArgsCount) {
            PrintHelpMessage();
        } else {
            GenerateParser(argv[1], argv[2], argv[3]);
        }
        return 0;
    } catch (errors::FilesystemError error) {
        std::cerr << "A filesystem error has occurred:\n";
        std::cerr << error.what() << "\n";
//    } catch (pg::TokenizerError error) {
//        pg::ErrorPrinter::Print(error);
//    } catch (pg::ParserError error) {
//        pg::ErrorPrinter::Print(error);
    } catch (std::runtime_error error) {
        std::cerr << "A runtime error has occurred:\n";
        std::cerr << error.what() << "\n";
    } catch (std::logic_error error) {
        std::cerr << "A logic error has occurred:\n";
        std::cerr << error.what() << "\n";
    } catch (...) {
        std::cerr << "An unknown error has occurred.\n";
    }
    return 1;
}

grammar::Symbol MakeT(const std::string& name) {
    return {grammar::Symbol::Type::Terminal, name};
}

grammar::Symbol MakeNT(const std::string& name) {
    return {grammar::Symbol::Type::NonTerminal, name};
}

void Application::GenerateParser(const std::string& action, const std::string& grammar_file,
                                 const std::string& output_path) {
    //    GrammarParser grammar_parser;
    //    grammar::Grammar grammar = grammar_parser.Parse(grammar_file);

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
                     {MakeT("At"), MakeNT("RuleHeader"), MakeNT("ProductionList")},
                     {false, true, true}});

    grammar.SetReturnType("RuleList", "RuleList");
    grammar.AddRule({"RuleList", "RuleListBegin", {MakeNT("Rule")}, {true}});
    grammar.AddRule(
        {"RuleList", "RuleListContinue", {MakeNT("RuleList"), MakeNT("Rule")}, {true, true}});

    grammar.SetMainRule("RuleList");

    grammar::GrammarInfo grammar_info(grammar);
    std::cerr << grammar_info << "\n";

    generators::SlrGenerator generator(grammar_info);
    generator.Visualize(std::cerr);

    generators::LrTables tables = generator.GenerateTables();
    code::CppGenerator code_gen(grammar_info, tables);

    if (action == "create") {
        code_gen.Create(output_path);
    } else if (action == "update") {
        code_gen.Update(output_path);
    } else {
        // TODO throw an error
    }
}

void Application::PrintHelpMessage() {
}
