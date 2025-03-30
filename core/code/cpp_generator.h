#pragma once

#include "../generators/slr_generator.h"

namespace code {

class CppGenerator {
private:
    static constexpr std::string kNamespace = "pg";
    static constexpr std::string kIdentifiersFilename = "identifiers.h";
    static constexpr std::string kIdentifierEnumName = "Identifier";
    static constexpr std::string kDataPath = "/data";
    static constexpr std::string kGrammarFilename = "grammar.h";
    static constexpr std::string kGrammarName = "kGrammar";
    static constexpr std::string kTokenTypeName = "TokenType";
    static constexpr std::string kTablesFilename = "tables.h";
    static constexpr std::string kActionTableName = "kActionTable";
    static constexpr std::string kGotoTableName = "kGotoTable";

public:
    CppGenerator(const grammar::GrammarInfo& grammar_info, const generators::LrTables& tables);

    void Generate(const std::string& path);

private:
    void GenerateIdentifiers(const std::string& path);
    void GenerateGrammar(const std::string& path);
    void GenerateTables(const std::string& path);
    void GenerateActionTable(std::ostream& out);
    void GenerateGotoTable(std::ostream& out);

private:
    std::ostream& WriteIdentifier(std::ostream& out, const std::string& name);
    std::ostream& WriteToken(std::ostream& out, const std::string& name);
    std::ostream& WriteSymbol(std::ostream& out, const grammar::Symbol& symbol);
    std::ostream& WriteLrAction(std::ostream& out, const generators::LrAction& action);
    std::ostream& WriteSizeTNumber(std::ostream& out, size_t number);

private:
    grammar::GrammarInfo grammar_info_;
    generators::LrTables tables_;
};

}  // namespace code
