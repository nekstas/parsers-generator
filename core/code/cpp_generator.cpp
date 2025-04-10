#include "cpp_generator.h"

#include <fstream>

namespace {

constexpr const char* kLibTokenHContent =
    "#pragma once\n"
    "\n"
    "#include <cstdint>\n"
    "#include <string>\n"
    "\n"
    "#include \"../data/token_type.h\"\n"
    "\n"
    "namespace pg {\n"
    "\n"
    "struct Token {\n"
    "    TokenType type;\n"
    "    std::string value;\n"
    "    size_t line;\n"
    "    size_t pos;\n"
    "\n"
    "    Token(TokenType type, const std::string& value, size_t line, size_t pos)\n"
    "        : type(type), value(value), line(line), pos(pos) {\n"
    "    }\n"
    "};\n"
    "\n"
    "}  // namespace pg";
constexpr const char* kLibLrDataHContent =
    "#pragma once\n"
    "\n"
    "#include <cstdint>\n"
    "#include <map>\n"
    "#include <string>\n"
    "#include <vector>\n"
    "\n"
    "#include \"../data/identifier.h\"\n"
    "#include \"../usr/lexer.h\"\n"
    "#include \"grammar.h\"\n"
    "\n"
    "namespace pg {\n"
    "\n"
    "struct LrAction {\n"
    "    enum class Type { SHIFT, REDUCE, ACCEPT, ERROR };\n"
    "    Type type;\n"
    "    size_t index;\n"
    "};\n"
    "\n"
    "using LrActionTable = std::vector<std::map<TokenType, LrAction>>;\n"
    "using LrGotoTable = std::vector<std::map<Identifier, size_t>>;\n"
    "\n"
    "struct LrData {\n"
    "    Grammar grammar;\n"
    "    LrActionTable action_table;\n"
    "    LrGotoTable goto_table;\n"
    "};\n"
    "\n"
    "}  // namespace pg";
constexpr const char* kLibGrammarHContent =
    "#pragma once\n"
    "\n"
    "#include <functional>\n"
    "#include <variant>\n"
    "#include <vector>\n"
    "\n"
    "#include \"../data/identifier.h\"\n"
    "#include \"../lib/ast_node.h\"\n"
    "#include \"../lib/token.h\"\n"
    "#include \"../usr/lexer.h\"\n"
    "\n"
    "namespace pg {\n"
    "\n"
    "using Symbol = std::variant<TokenType, Identifier>;\n"
    "\n"
    "class AstBuilder;\n"
    "\n"
    "struct Rule {\n"
    "    using HandlerArg = std::variant<Token, ast::NodePtr>;\n"
    "    using HandlerArgs = std::vector<HandlerArg>;\n"
    "    using HandlerType =\n"
    "        std::function<ast::NodePtr(AstBuilder& ast_builder, const HandlerArgs& args)>;\n"
    "\n"
    "    Identifier result;\n"
    "    std::vector<Symbol> sequence;\n"
    "    HandlerType handler;\n"
    "};\n"
    "\n"
    "struct Grammar {\n"
    "    std::vector<Rule> rules;\n"
    "};\n"
    "\n"
    "}  // namespace pg";
constexpr const char* kLibAstNodeHContent =
    "#pragma once\n"
    "\n"
    "#include <memory>\n"
    "#include <stdexcept>\n"
    "\n"
    "namespace ast {\n"
    "\n"
    "class Node {\n"
    "public:\n"
    "    virtual ~Node() {\n"
    "    }\n"
    "};\n"
    "\n"
    "using NodePtr = std::shared_ptr<Node>;\n"
    "\n"
    "template <typename T, typename... Args>\n"
    "std::shared_ptr<T> Make(Args&&... args) {\n"
    "    return std::make_shared<T>(std::forward<Args>(args)...);\n"
    "}\n"
    "\n"
    "template <typename T>\n"
    "bool Is(NodePtr node) {\n"
    "    return static_cast<bool>(std::dynamic_pointer_cast<T>(node));\n"
    "}\n"
    "\n"
    "template <typename T>\n"
    "std::shared_ptr<T> As(NodePtr node) {\n"
    "    std::shared_ptr<T> result = std::dynamic_pointer_cast<T>(node);\n"
    "    if (!result) {\n"
    "        throw std::logic_error{\"Can't cast to given node type.\"};\n"
    "    }\n"
    "    return std::dynamic_pointer_cast<T>(node);\n"
    "}\n"
    "\n"
    "}  // namespace ast";
constexpr const char* kLibErrorsHContent =
    "#pragma once\n"
    "\n"
    "#include <stdexcept>\n"
    "#include <string>\n"
    "#include <vector>\n"
    "\n"
    "#include \"tokenizer.h\"\n"
    "\n"
    "namespace pg {\n"
    "\n"
    "class TokenizerError : public std::exception {\n"
    "public:\n"
    "    TokenizerError(const std::string& message, const std::vector<std::string>& lines, size_t "
    "line,\n"
    "                   size_t pos)\n"
    "        : message(message), line_content(lines[line]), line(line), pos(pos) {\n"
    "    }\n"
    "\n"
    "    std::string message;\n"
    "    std::string line_content;\n"
    "    size_t line;\n"
    "    size_t pos;\n"
    "};\n"
    "\n"
    "class ParserError : public std::exception {\n"
    "public:\n"
    "    ParserError(const std::string& message, const pg::Tokenizer::Result& input, size_t "
    "error_index)\n"
    "        : message(message) {\n"
    "        const Token& token = input.tokens[error_index];\n"
    "        line_content = input.lines[token.line];\n"
    "        std::tie(line, pos, len) = std::make_tuple(token.line, token.pos, "
    "token.value.size());\n"
    "    }\n"
    "\n"
    "    std::string message;\n"
    "    std::string line_content;\n"
    "    size_t line;\n"
    "    size_t pos;\n"
    "    size_t len;\n"
    "};\n"
    "\n"
    "}  // namespace pg";
constexpr const char* kLibTokenizerHContent =
    "#pragma once\n"
    "\n"
    "#include <cstdint>\n"
    "#include <string>\n"
    "#include <vector>\n"
    "\n"
    "#include \"../usr/lexer.h\"\n"
    "#include \"token.h\"\n"
    "\n"
    "namespace pg {\n"
    "\n"
    "class Tokenizer {\n"
    "private:\n"
    "    struct Context {\n"
    "        const char* prev = nullptr;\n"
    "        const char* current = nullptr;\n"
    "        const char* end = nullptr;\n"
    "        const char* line_begin = nullptr;\n"
    "        size_t line_number = 0;\n"
    "\n"
    "        Context() = default;\n"
    "        explicit Context(const std::string& code)\n"
    "            : prev(code.c_str()),\n"
    "              current(prev),\n"
    "              end(prev + code.size()),\n"
    "              line_begin(prev),\n"
    "              line_number(0) {\n"
    "        }\n"
    "    };\n"
    "\n"
    "public:\n"
    "    struct Result {\n"
    "        std::vector<pg::Token> tokens;\n"
    "        std::vector<std::string> lines;\n"
    "    };\n"
    "\n"
    "public:\n"
    "    Result Tokenize(const std::string& code);\n"
    "\n"
    "private:\n"
    "    void Init(const std::string& code);\n"
    "    void SplitIntoLines();\n"
    "    bool IsEnd();\n"
    "    void ProcessNewToken();\n"
    "    void ProcessNewLines();\n"
    "    void AddEofToken();\n"
    "    size_t GetCurrentLine();\n"
    "    size_t GetCurrentPos();\n"
    "\n"
    "private:\n"
    "    pg::Lexer lexer_;\n"
    "    Context context_;\n"
    "    Result result_;\n"
    "};\n"
    "\n"
    "}  // namespace pg";
constexpr const char* kLibTokenizerCppContent =
    "#include \"tokenizer.h\"\n"
    "\n"
    "#include \"errors.h\"\n"
    "\n"
    "pg::Tokenizer::Result pg::Tokenizer::Tokenize(const std::string& code) {\n"
    "    Init(code);\n"
    "    SplitIntoLines();\n"
    "\n"
    "    while (!IsEnd()) {\n"
    "        ProcessNewToken();\n"
    "        ProcessNewLines();\n"
    "    }\n"
    "\n"
    "    AddEofToken();\n"
    "    return result_;\n"
    "}\n"
    "\n"
    "void pg::Tokenizer::Init(const std::string& code) {\n"
    "    lexer_ = pg::Lexer{};\n"
    "    context_ = Context{code};\n"
    "    result_ = Result{};\n"
    "}\n"
    "\n"
    "void pg::Tokenizer::SplitIntoLines() {\n"
    "    const char* prev = context_.current;\n"
    "    for (const char* current = context_.current; current != context_.end; ++current) {\n"
    "        if (*current == '\\n') {\n"
    "            result_.lines.emplace_back(prev, current);\n"
    "            prev = current + 1;\n"
    "        }\n"
    "    }\n"
    "    result_.lines.emplace_back(prev, context_.end);\n"
    "}\n"
    "\n"
    "bool pg::Tokenizer::IsEnd() {\n"
    "    return context_.current == context_.end;\n"
    "}\n"
    "\n"
    "void pg::Tokenizer::ProcessNewToken() {\n"
    "    pg::TokenType type = lexer_.GetTokenType(context_.current, context_.end);\n"
    "    if (type == pg::TokenType::Unknown) {\n"
    "        throw pg::TokenizerError(\"unknown token\", result_.lines, GetCurrentLine(), "
    "GetCurrentPos());\n"
    "    } else if (type == pg::TokenType::Eof) {\n"
    "        throw std::logic_error{\"Lexer should not return Eof token.\"};\n"
    "    } else if (type != pg::TokenType::Skip) {\n"
    "        result_.tokens.emplace_back(type, std::string(context_.prev, context_.current),\n"
    "                                    GetCurrentLine(), GetCurrentPos());\n"
    "    }\n"
    "}\n"
    "\n"
    "void pg::Tokenizer::ProcessNewLines() {\n"
    "    for (; context_.prev != context_.current; ++context_.prev) {\n"
    "        if (*context_.prev == '\\n') {\n"
    "            context_.line_begin = context_.prev + 1;\n"
    "            ++context_.line_number;\n"
    "        }\n"
    "    }\n"
    "}\n"
    "\n"
    "void pg::Tokenizer::AddEofToken() {\n"
    "    result_.tokens.emplace_back(pg::TokenType::Eof, \"\", GetCurrentLine(), "
    "GetCurrentPos());\n"
    "}\n"
    "\n"
    "size_t pg::Tokenizer::GetCurrentLine() {\n"
    "    return context_.line_number;\n"
    "}\n"
    "\n"
    "size_t pg::Tokenizer::GetCurrentPos() {\n"
    "    return context_.prev - context_.line_begin;\n"
    "}";
constexpr const char* kLibParserHContent =
    "#pragma once\n"
    "\n"
    "#include \"../data/ast_builder.h\"\n"
    "#include \"../usr/lexer.h\"\n"
    "#include \"lr_data.h\"\n"
    "#include \"tokenizer.h\"\n"
    "\n"
    "namespace pg {\n"
    "\n"
    "class LrParser {\n"
    "public:\n"
    "    LrParser(const LrData& tables);\n"
    "\n"
    "    void Parse(const Tokenizer::Result& input, AstBuilder& ast_builder) const;\n"
    "    void Parse(const std::string& code, AstBuilder& ast_builder) const;\n"
    "\n"
    "    static LrParser Create();\n"
    "\n"
    "private:\n"
    "    LrData data_;\n"
    "};\n"
    "\n"
    "}  // namespace pg";
constexpr const char* kLibParserCppContent =
    "#include \"parser.h\"\n"
    "\n"
    "#include \"../data/grammar.h\"\n"
    "#include \"../data/tables.h\"\n"
    "#include \"errors.h\"\n"
    "\n"
    "pg::LrParser::LrParser(const pg::LrData& data) : data_(data) {\n"
    "}\n"
    "\n"
    "void pg::LrParser::Parse(const pg::Tokenizer::Result& input, pg::AstBuilder& ast_builder) "
    "const {\n"
    "    const auto& tokens = input.tokens;\n"
    "\n"
    "    if (tokens.empty() || tokens.back().type != TokenType::Eof) {\n"
    "        throw std::logic_error{\"The parser received an incorrect list of tokens.\"};\n"
    "    }\n"
    "\n"
    "    std::vector<size_t> states = {0};\n"
    "    std::vector<Rule::HandlerArg> result_stack;\n"
    "    for (size_t i = 0; i < tokens.size() && !states.empty();) {\n"
    "        size_t current_state = states.back();\n"
    "        Token current_token = tokens.at(i);\n"
    "\n"
    "        LrAction action = data_.action_table.at(current_state).at(current_token.type);\n"
    "        if (action.type == LrAction::Type::SHIFT) {\n"
    "            states.push_back(action.index);\n"
    "            result_stack.push_back(current_token);\n"
    "            ++i;\n"
    "        } else if (action.type == LrAction::Type::REDUCE) {\n"
    "            const Rule& rule = data_.grammar.rules.at(action.index);\n"
    "            size_t k = rule.sequence.size();\n"
    "            Rule::HandlerArgs args(result_stack.end() - k, result_stack.end());\n"
    "            for (size_t j = 0; j < k; ++j) {\n"
    "                states.pop_back();\n"
    "                result_stack.pop_back();\n"
    "            }\n"
    "            states.push_back(data_.goto_table.at(states.back()).at(rule.result));\n"
    "            result_stack.push_back(rule.handler(ast_builder, args));\n"
    "        } else {\n"
    "            if (action.type == LrAction::Type::ACCEPT) {\n"
    "                "
    "ast_builder.Accept(kToMainRule(std::get<ast::NodePtr>(result_stack.back())));\n"
    "                break;\n"
    "            }\n"
    "            throw pg::ParserError(\"unexpected token\", input, i);\n"
    "        }\n"
    "    }\n"
    "}\n"
    "\n"
    "void pg::LrParser::Parse(const std::string& code, pg::AstBuilder& ast_builder) const {\n"
    "    pg::Tokenizer::Result result = pg::Tokenizer().Tokenize(code);\n"
    "    Parse(result, ast_builder);\n"
    "}\n"
    "\n"
    "pg::LrParser pg::LrParser::Create() {\n"
    "    return pg::LrParser({kGrammar, kActionTable, kGotoTable});\n"
    "}";
constexpr const char* kLibErrorPrinterHContent =
    "#pragma once\n"
    "\n"
    "#include \"errors.h\"\n"
    "#include \"tokenizer.h\"\n"
    "\n"
    "namespace pg {\n"
    "\n"
    "class ErrorPrinter {\n"
    "public:\n"
    "    static void SourceCodeError(const std::string& message, const std::string& line_content,\n"
    "                                size_t line, size_t pos, size_t len);\n"
    "\n"
    "    static void Print(const TokenizerError& error);\n"
    "    static void Print(const ParserError& error);\n"
    "};\n"
    "\n"
    "}  // namespace pg";
constexpr const char* kLibErrorPrinterCppContent =
    "#include \"error_printer.h\"\n"
    "\n"
    "#include <iostream>\n"
    "\n"
    "void pg::ErrorPrinter::SourceCodeError(const std::string& message, const std::string& "
    "line_content,\n"
    "                                       size_t line, size_t pos, size_t len) {\n"
    "    std::cerr << \"An error has occurred: \" << message;\n"
    "    std::cerr << \" on line \" << (line + 1) << \", character \" << (pos + 1) << \"\\n\";\n"
    "    std::cerr << \"| \" << line_content << \"\\n\";\n"
    "    std::cerr << \"| \" << std::string(pos, ' ') << std::string(len, '^') << \"\\n\";\n"
    "}\n"
    "\n"
    "void pg::ErrorPrinter::Print(const pg::TokenizerError& error) {\n"
    "    SourceCodeError(error.message, error.line_content, error.line, error.pos, 1);\n"
    "}\n"
    "\n"
    "void pg::ErrorPrinter::Print(const pg::ParserError& error) {\n"
    "    SourceCodeError(error.message, error.line_content, error.line, error.pos, error.len);\n"
    "}";

constexpr const char* kUsrLexerHContent =
    "#pragma once\n"
    "\n"
    "#include <cstdint>\n"
    "#include <string>\n"
    "\n"
    "#include \"../data/token_type.h\"\n"
    "\n"
    "namespace pg {\n"
    "\n"
    "class Lexer {\n"
    "public:\n"
    "    TokenType GetTokenType(const char*& current, const char* end);\n"
    "};\n"
    "\n"
    "}  // namespace pg";

constexpr const char* kUsrLexerReContent =
    "#include \"lexer.h\"\n"
    "\n"
    "pg::TokenType pg::Lexer::GetTokenType(const char*& str_ptr, const char* end) {\n"
    "    const char* marker = nullptr;\n"
    "\n"
    "    // NOLINTBEGIN\n"
    "    /*!re2c\n"
    "        re2c:define:YYCTYPE = \"unsigned char\";\n"
    "        re2c:define:YYCURSOR = \"str_ptr\";\n"
    "        re2c:define:YYLIMIT = \"end\";\n"
    "        re2c:define:YYMARKER = \"marker\";\n"
    "        re2c:yyfill:enable = 0;\n"
    "\n"
    "        * { return TokenType::Unknown; }\n"
    "\n"
    "        // Your rules here...\n"
    "\n"
    "    */\n"
    "    // NOLINTEND\n"
    "\n"
    "    return TokenType::Unknown;\n"
    "}";

}  // namespace

code::CppGenerator::CppGenerator(const grammar::GrammarInfo& grammar_info,
                                 const generators::LrTables& tables)
    : grammar_info_(grammar_info), grammar_(grammar_info.GetGrammar()), tables_(tables) {
}

void code::CppGenerator::Create(const std::string& path) {
    SetPath(path);
    GenerateLibFiles();
    GenerateDataFiles();
    GenerateUsrFiles();
}

void code::CppGenerator::Update(const std::string& path) {
    SetPath(path);
    GenerateDataFiles();
}

void code::CppGenerator::GenerateLibFiles() {
    auto dir = WithDir("lib");
    WriteContent("token.h", kLibTokenHContent);
    WriteContent("lr_data.h", kLibLrDataHContent);
    WriteContent("grammar.h", kLibGrammarHContent);
    WriteContent("ast_node.h", kLibAstNodeHContent);
    WriteContent("errors.h", kLibErrorsHContent);
    WriteContent("tokenizer.h", kLibTokenizerHContent);
    WriteContent("tokenizer.cpp", kLibTokenizerCppContent);
    WriteContent("parser.h", kLibParserHContent);
    WriteContent("parser.cpp", kLibParserCppContent);
    WriteContent("error_printer.h", kLibErrorPrinterHContent);
    WriteContent("error_printer.cpp", kLibErrorPrinterCppContent);
}

void code::CppGenerator::GenerateDataFiles() {
    auto dir = WithDir("data");
    GenerateIdentifierFile();
    GenerateTokenTypeFile();
    GenerateGrammarFile();
    GenerateTablesFile();
    GenerateAstBuilderFile();
}

void code::CppGenerator::GenerateUsrFiles() {
    auto dir = WithDir("usr");
    WriteContent("lexer.h", kUsrLexerHContent);
    WriteContent("lexer.re", kUsrLexerReContent);
    GenerateAstNodesFile();
}

void code::CppGenerator::WriteContent(const std::string& filename, const std::string& content) {
    auto out = OpenFile(filename);
    out << content;
}

void code::CppGenerator::GenerateIdentifierFile() {
    const std::string& main_rule = grammar_info_.GetGrammar().GetMainRule();
    auto values = grammar_info_.GetUsedRules();
    values.erase(main_rule);
    GenerateEnumFile(kIdentifierFilename, kIdentifierEnumName, values);
}

void code::CppGenerator::GenerateTokenTypeFile() {
    auto values = grammar_info_.GetUsedTokens();
    values.insert(kSkipTokenName);
    values.insert(kUnknownTokenName);
    GenerateEnumFile(kTokenTypeFilename, kTokenTypeEnumName, values);
}

void code::CppGenerator::GenerateGrammarFile() {
    auto out = OpenFile(kGrammarFilename);
    out << "#pragma once\n";
    out << "#include \"../lib/grammar.h\"\n";
    out << "#include \"" << kIdentifierFilename << "\"\n";
    out << "#include \"" << kAstBuilderFilename << "\"\n\n";
    out << "namespace " << kMainNamespace << " {\n";
    out << "const " << kGrammarStructName << " " << kGrammarName << " = {{\n";
    const auto& rules = grammar_.GetRules();
    for (const auto& rule : rules) {
        if (rule.name == grammar::GrammarInfo::kNewMainRuleName) {
            continue;
        }

        out << "{";
        WriteIdentifier(out, rule.name);
        out << ", {";
        for (const grammar::Symbol& symbol : rule.sequence) {
            WriteSymbol(out, symbol);
            out << ", ";
        }
        out << "},";
        out << "[](" << kAstBuilderClassName << "& ast_builder, ";
        out << "const " << kHandlerArgs << "& args){";
        out << "return ast_builder.";
        WriteMethodForRuleName(out, rule);

        out << "(";
        bool is_first = true;
        for (size_t i = 0; i < rule.sequence.size(); ++i) {
            if (!rule.take[i]) {
                continue;
            }
            if (!is_first) {
                out << ", ";
            }
            if (rule.sequence[i].type == grammar::Symbol::Type::Terminal) {
                out << "std::get<" << kTokenStruct << ">(args[" << i << "])";
            } else {
                out << "ast::As<";
                WriteMethodReturnType(out, rule.sequence[i].value);
                out << ">(";
                out << "std::get<" << kAstNodePtr << ">(args[" << i << "])";
                out << ")";
            }
            is_first = false;
        }
        out << ");";

        out << "}";
        out << "},\n";
    }
    out << "}};\n\n";

    out << "const auto kToMainRule = [](ast::NodePtr node) { return ast::As<";
    WriteMethodReturnType(out, grammar_info_.GetOldMainRuleName());
    out << ">(node); };\n\n";

    out << "}\n";
}

void code::CppGenerator::GenerateTablesFile() {
    auto out = OpenFile(kTablesFilename);
    out << "#pragma once\n";
    out << "#include \"../lib/lr_data.h\"\n";
    out << "#include \"" << kIdentifierFilename << "\"\n\n";
    out << "namespace " << kMainNamespace << " {\n";
    GenerateActionTable(out);
    GenerateGotoTable(out);
    out << "}";
}

void code::CppGenerator::GenerateActionTable(std::ostream& out) {
    out << "const " << kActionTableStructName << " " << kActionTableName << " = {\n";
    for (size_t i = 0; i < tables_.action_table.size(); ++i) {
        out << "{";
        for (const auto& [name, action] : tables_.action_table[i]) {
            out << "{";
            WriteToken(out, name);
            out << ", ";
            WriteLrAction(out, action);
            out << "}, ";
        }
        out << "}, ";
    }
    out << "\n};\n";
}

void code::CppGenerator::GenerateGotoTable(std::ostream& out) {
    out << "const " << kGotoTableStructName << " " << kGotoTableName << " = {\n";
    for (size_t i = 0; i < tables_.goto_table.size(); ++i) {
        out << "{";
        for (const auto& [name, new_state] : tables_.goto_table[i]) {
            out << "{";
            WriteIdentifier(out, name);
            out << ", ";
            WriteSizeTNumber(out, new_state);
            out << "}, ";
        }
        out << "}, ";
    }
    out << "\n};\n";
}

void code::CppGenerator::WriteIdentifier(std::ostream& out, const std::string& name) {
    out << kIdentifierEnumName << "::" << name;
}

void code::CppGenerator::WriteToken(std::ostream& out, const std::string& name) {
    out << kTokenTypeEnumName << "::" << name;
}

void code::CppGenerator::WriteSymbol(std::ostream& out, const grammar::Symbol& symbol) {
    if (symbol.type == grammar::Symbol::Type::Terminal) {
        WriteToken(out, symbol.value);
    } else {
        WriteIdentifier(out, symbol.value);
    }
}

void code::CppGenerator::WriteLrAction(std::ostream& out, const generators::LrAction& action) {
    out << "{";
    switch (action.type) {
        case generators::LrAction::Type::SHIFT:
            out << "LrAction::Type::SHIFT";
            break;
        case generators::LrAction::Type::REDUCE:
            out << "LrAction::Type::REDUCE";
            break;
        case generators::LrAction::Type::ACCEPT:
            out << "LrAction::Type::ACCEPT";
            break;
        case generators::LrAction::Type::ERROR:
            out << "LrAction::Type::ERROR";
            break;
    }
    out << ", ";
    WriteSizeTNumber(out, action.index);
    out << "}";
}

void code::CppGenerator::WriteSizeTNumber(std::ostream& out, size_t number) {
    if (number == -1) {
        out << "-1";
    } else {
        out << number;
    }
}

void code::CppGenerator::GenerateAstBuilderFile() {
    auto out = OpenFile(kAstBuilderFilename);
    out << "#pragma once\n";
    out << "#include <vector>\n";
    out << "#include \"../lib/ast_node.h\"\n";
    out << "#include \"../lib/grammar.h\"\n";
    out << "#include \"../lib/token.h\"\n";
    out << "#include \"../lib/tokenizer.h\"\n";
    out << "#include \"../lib/error_printer.h\"\n\n";
    out << "#include \"../usr/ast_nodes.h\"\n\n";

    out << "namespace " << kMainNamespace << " {\n";
    out << "class " << kAstBuilderClassName << " {\n";
    out << "public:\n";
    out << "    virtual ~" << kAstBuilderClassName << "() {}\n\n";
    out << "    virtual void Setup() {};\n";
    out << "    virtual void Accept(";
    WriteMethodReturnTypePtr(out, grammar_info_.GetOldMainRuleName());
    out << " root) = 0;\n\n";

    const auto& rules_map = grammar_.GetRulesMap();
    for (const auto& [identifier, rules] : rules_map) {
        if (identifier == grammar_.GetMainRule()) {
            continue;
        }
        for (size_t rule_index : rules) {
            const auto& rule = grammar_.GetRule(rule_index);
            out << "    virtual ";
            WriteMethodReturnTypePtr(out, rule.name);
            out << " ";
            WriteMethodForRuleName(out, rule);

            out << "(";
            bool is_first = true;
            for (size_t i = 0; i < rule.sequence.size(); ++i) {
                if (!rule.take[i]) {
                    continue;
                }
                if (!is_first) {
                    out << ", ";
                }
                if (rule.sequence[i].type == grammar::Symbol::Type::Terminal) {
                    out << "const Token&";
                } else {
                    WriteMethodReturnTypePtr(out, rule.sequence[i].value);
                }
                is_first = false;
            }
            out << ") = 0;\n";
        }
    }

    out << "};\n";
    out << "}";
}

void code::CppGenerator::GenerateEnumFile(const std::string& filename, const std::string& enum_name,
                                          const std::set<std::string>& values) {
    auto out = OpenFile(filename);
    out << "#pragma once\n";
    out << "#include <cstdint>\n\n";
    out << "namespace " << kMainNamespace << " {\n";
    out << "enum class " << enum_name << " : size_t { ";
    for (const std::string& name : values) {
        out << name << ", ";
    }
    out << "};\n";
    out << "}\n";
}

void code::CppGenerator::WriteMethodForRuleName(std::ostream& out, const grammar::Rule& rule) {
    out << "Handle" << rule.handler_name;
}

void code::CppGenerator::WriteMethodReturnType(std::ostream& out, const std::string& name) {
    out << kAstNamespace << "::" << grammar_info_.GetGrammar().GetReturnType(name);
}

void code::CppGenerator::WriteMethodReturnTypePtr(std::ostream& out, const std::string& name) {
    out << "std::shared_ptr<";
    WriteMethodReturnType(out, name);
    out << ">";
}

void code::CppGenerator::GenerateAstNodesFile() {
    auto out = OpenFile("ast_nodes.h");
    out << "#pragma once\n\n";
    out << "#include \"../lib/ast_node.h\"\n\n";
    out << "namespace " << kAstNamespace << " {\n\n";

    for (const std::string& return_type : grammar_info_.GetUsedReturnTypes()) {
        out << "class " << return_type << " : public " << kAstNode << " {};\n";
    }

    out << "}\n";
}
