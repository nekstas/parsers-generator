#pragma once

#include "../generators/slr_generator.h"
#include "code_generator.h"
#include "filesystem_helper.h"

namespace code {

class CppGenerator : public CodeGenerator, public fs::FilesystemHelper {
private:
    static constexpr std::string kMainNamespace = "pg";
    static constexpr std::string kAstNamespace = "ast";

    static constexpr std::string kIdentifierFilename = "identifier.h";
    static constexpr std::string kIdentifierEnumName = "Identifier";

    static constexpr std::string kTokenTypeFilename = "token_type.h";
    static constexpr std::string kTokenTypeEnumName = "TokenType";
    static constexpr std::string kSkipTokenName = "Skip";
    static constexpr std::string kUnknownTokenName = "Unknown";

    static constexpr std::string kGrammarFilename = "grammar.h";
    static constexpr std::string kGrammarStructName = "Grammar";
    static constexpr std::string kHandlerArgs = "Rule::HandlerArgs";
    static constexpr std::string kGrammarName = "kGrammar";

    static constexpr std::string kTablesFilename = "tables.h";

    static constexpr std::string kActionTableStructName = "LrActionTable";
    static constexpr std::string kActionTableName = "kActionTable";

    static constexpr std::string kGotoTableStructName = "LrGotoTable";
    static constexpr std::string kGotoTableName = "kGotoTable";

    static constexpr std::string kAstBuilderFilename = "ast_builder.h";
    static constexpr std::string kAstBuilderClassName = "AstBuilder";
    static constexpr std::string kAstNodePtr = "ast::AstNodePtr";
    static constexpr std::string kTokenStruct = "Token";

public:
    CppGenerator(const grammar::GrammarInfo& grammar_info, const generators::LrTables& tables);

    void Create(const std::string& path);

private:
    void GenerateLibFiles();
    void GenerateDataFiles();
    void GenerateUsrFiles();

private:
    void GenerateIdentifierFile();
    void GenerateTokenTypeFile();
    void GenerateGrammarFile();
    void GenerateTablesFile();
    void GenerateAstBuilderFile();
    void GenerateActionTable(std::ostream& out);
    void GenerateGotoTable(std::ostream& out);
    void GenerateEnumFile(const std::string& path, const std::string& enum_name,
                          const std::set<std::string>& values);

private:
    void WriteIdentifier(std::ostream& out, const std::string& name);
    void WriteToken(std::ostream& out, const std::string& name);
    void WriteSymbol(std::ostream& out, const grammar::Symbol& symbol);
    void WriteLrAction(std::ostream& out, const generators::LrAction& action);
    void WriteSizeTNumber(std::ostream& out, size_t number);
    void WriteMethodForRuleName(std::ostream& out, const grammar::Rule& rule);
    void WriteMethodReturnType(std::ostream& out, const std::string& name);
    void WriteMethodReturnTypePtr(std::ostream& out, const std::string& name);

private:
    grammar::GrammarInfo grammar_info_;
    generators::LrTables tables_;
};

}  // namespace code
