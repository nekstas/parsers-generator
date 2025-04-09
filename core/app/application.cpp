#include "application.h"

#include "../../utils/format_stream.h"
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
    } catch (pg::TokenizerError error) {
        pg::ErrorPrinter::Print(error);
    } catch (pg::ParserError error) {
        pg::ErrorPrinter::Print(error);
    } catch (errors::ApplicationError error) {
        std::cerr << "An error has occurred:\n";
        std::cerr << error.what() << "\n";
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

void Application::GenerateParser(const std::string& action, const std::string& grammar_file,
                                 const std::string& output_path) {
    GrammarParser grammar_parser;
    grammar::Grammar grammar = grammar_parser.Parse(grammar_file);

    grammar::GrammarInfo grammar_info(grammar);
    generators::SlrGenerator generator(grammar_info);

    generators::LrTables tables = generator.GenerateTables();
    code::CppGenerator code_gen(grammar_info, tables);

    if (action == "create") {
        code_gen.Create(output_path);
    } else if (action == "update") {
        code_gen.Update(output_path);
    } else {
        throw errors::ApplicationError{FormatStream()
                                       << "Unknown action \"" << action
                                       << "\". Available actions: \"create\" or \"update\"."};
    }
}

void Application::PrintHelpMessage() {
}
