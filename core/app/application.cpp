#include "application.h"

#include "../../utils/filesystem.h"
#include "../../utils/format_stream.h"
#include "../code/cpp_generator.h"
#include "../grammar_parser/lib/parser.h"
#include "../grammar_parser/usr/grammar_builder.h"
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
    std::string code = fs::ReadFile(grammar_file);
    GrammarBuilder builder;
    pg::LrParser::Create().Parse(code, builder);
    grammar::Grammar grammar = builder.GetResult();

    grammar::GrammarInfo grammar_info(grammar);
    generators::SlrGenerator generator(grammar_info);

    generators::LrTables tables = generator.GenerateTables();
    code::CppGenerator code_gen(grammar_info, tables);

    if (action == "create") {
        code_gen.Create(output_path);
        std::cout << "Parser for a grammar \n\"" << grammar_file
                  << "\"\nhas been successfully created in the directory\n\"" << output_path
                  << "\".";
    } else if (action == "update") {
        code_gen.Update(output_path);
        std::cout << "Parser for a grammar \n\"" << grammar_file
                  << "\"\nhas been successfully updated in the directory\n\"" << output_path
                  << "\".";
    } else {
        throw errors::ApplicationError{FormatStream()
                                       << "Unknown action \"" << action
                                       << "\". Available actions: \"create\" or \"update\"."};
    }
}

void Application::PrintHelpMessage() {
    std::cout << "Parsers Generator help message.\n";
    std::cout << "Usage: parsers_generator <action> <grammar_file> <output_path>\n";
    std::cout << "- <action> can be 'create' to create a parser for the first time or 'update' to "
                 "update an existing parser.\n";
    std::cout << "- <grammar_path> is a path to file with grammar of your language.\n";
    std::cout << "- <output_path> is a path to output directory.\n";
    std::cout
        << "Check syntax of grammar file and generated parser structure on project's GitHub.\n";
    std::cout << "It's recommended to use re2c to write a lexer for your language.\n\n";
    std::cout << "Project on GitHub: https://github.com/nekstas/parsers-generator\n";
    std::cout << "Author: @nekstas (Nekrasov Stanislav)\n";
}
