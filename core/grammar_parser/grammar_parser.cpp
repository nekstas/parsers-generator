#include "grammar_parser.h"

#include "../../utils/filesystem.h"
#include "lib/parser.h"
#include "usr/grammar_builder.h"

grammar::Grammar GrammarParser::Parse(const std::string& path) const {
    std::string code = fs::ReadFile(path);
    pg::Tokenizer::Result result = pg::Tokenizer().Tokenize(code);
    pg::LrParser parser = pg::LrParser::Create();
    GrammarBuilder builder;
    parser.Parse(result, builder);
    return builder.GetResult();
}
