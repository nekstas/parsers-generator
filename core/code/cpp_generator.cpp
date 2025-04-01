#include "cpp_generator.h"

#include <fstream>

code::CppGenerator::CppGenerator(const grammar::GrammarInfo& grammar_info,
                                 const generators::LrTables& tables)
    : grammar_info_(grammar_info), tables_(tables) {
}

void code::CppGenerator::Generate(const std::string& path) {
    std::string data_path = path + kDataPath;
    GenerateIdentifierFile(data_path);
    GenerateTokenTypeFile(data_path);
    GenerateGrammarFile(data_path);
    GenerateTablesFile(data_path);
    GenerateAstBuilderFile(data_path);
}

void code::CppGenerator::GenerateIdentifierFile(const std::string& path) {
    const std::string& main_rule = grammar_info_.GetGrammar().GetMainRule();
    auto values = grammar_info_.GetUsedRules();
    values.erase(main_rule);
    GenerateEnumFile(path + "/" + kIdentifierFilename, kIdentifierEnumName, values);
}

void code::CppGenerator::GenerateTokenTypeFile(const std::string& path) {
    auto values = grammar_info_.GetUsedTokens();
    values.insert(kSkipTokenName);
    values.insert(kUnknownTokenName);
    GenerateEnumFile(path + "/" + kTokenTypeFilename, kTokenTypeEnumName, values);
}

void code::CppGenerator::GenerateGrammarFile(const std::string& path) {
    std::ofstream out(path + "/" + kGrammarFilename);
    out << "#pragma once\n";
    out << "#include \"../lib/grammar.h\"\n";
    out << "#include \"" << kIdentifierFilename << "\"\n\n";
    out << "namespace " << kNamespace << " {\n";
    out << "const Grammar " << kGrammarName << " = {{\n";
    for (const grammar::Rule& rule : grammar_info_.GetGrammar().GetRules()) {
        if (rule.name == grammar::GrammarInfo::kNewMainRuleName) {
            continue;
        }

        out << "{";
        WriteIdentifier(out, rule.name);
        out << ", \"" << rule << "\", {";
        for (const grammar::Symbol& symbol : rule.sequence) {
            WriteSymbol(out, symbol) << ", ";
        }
        out << "}},\n";
    }
    out << "}};\n";
    out << "}\n";
}

void code::CppGenerator::GenerateTablesFile(const std::string& path) {
    std::ofstream out(path + "/" + kTablesFilename);
    out << "#pragma once\n";
    out << "#include \"../lib/lr_data.h\"\n";
    out << "#include \"" << kIdentifierFilename << "\"\n\n";
    out << "namespace " << kNamespace << " {\n";
    GenerateActionTable(out);
    GenerateGotoTable(out);
    out << "}";
}

void code::CppGenerator::GenerateActionTable(std::ostream& out) {
    out << "const LrActionTable " << kActionTableName << " = {\n";
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
    out << "const LrGotoTable " << kGotoTableName << " = {\n";
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

std::ostream& code::CppGenerator::WriteIdentifier(std::ostream& out, const std::string& name) {
    out << kIdentifierEnumName << "::" << name;
    return out;
}

std::ostream& code::CppGenerator::WriteToken(std::ostream& out, const std::string& name) {
    out << kTokenTypeEnumName << "::" << name;
    return out;
}

std::ostream& code::CppGenerator::WriteSymbol(std::ostream& out, const grammar::Symbol& symbol) {
    if (symbol.type == grammar::Symbol::Type::Terminal) {
        return WriteToken(out, symbol.value);
    } else {
        return WriteIdentifier(out, symbol.value);
    }
}

std::ostream& code::CppGenerator::WriteLrAction(std::ostream& out,
                                                const generators::LrAction& action) {
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
    return out;
}

std::ostream& code::CppGenerator::WriteSizeTNumber(std::ostream& out, size_t number) {
    if (number == -1) {
        return out << "-1";
    } else {
        return out << number;
    }
}

void code::CppGenerator::GenerateAstBuilderFile(const std::string& path) {
}

void code::CppGenerator::GenerateEnumFile(const std::string& path, const std::string& enum_name,
                                          const std::set<std::string>& values) {
    std::ofstream out(path);
    out << "#pragma once\n";
    out << "#include <cstdint>\n\n";
    out << "namespace " << kNamespace << " {\n";
    out << "enum class " << enum_name << " : size_t { ";
    for (const std::string& name : values) {
        out << name << ", ";
    }
    out << "};\n";
    out << "}\n";
}
