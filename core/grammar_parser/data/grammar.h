#pragma once
#include "../lib/grammar.h"
#include "ast_builder.h"
#include "identifier.h"

namespace pg {
const Grammar kGrammar = {{
    {Identifier::Symbol,
     {
         TokenType::Name,
     },
     [](AstBuilder& ast_builder, const Rule::HandlerArgs& args) {
         return ast_builder.HandleTerminal(std::get<Token>(args[0]));
     }},
    {Identifier::Symbol,
     {
         TokenType::OpenTr,
         TokenType::Name,
         TokenType::CloseTr,
     },
     [](AstBuilder& ast_builder, const Rule::HandlerArgs& args) {
         return ast_builder.HandleNonTerminal(std::get<Token>(args[1]));
     }},
    {Identifier::TakeSymbol,
     {
         Identifier::Symbol,
     },
     [](AstBuilder& ast_builder, const Rule::HandlerArgs& args) {
         return ast_builder.HandleDontTakeSymbol(
             ast::As<ast::Symbol>(std::get<ast::NodePtr>(args[0])));
     }},
    {Identifier::TakeSymbol,
     {
         TokenType::Dollar,
         Identifier::Symbol,
     },
     [](AstBuilder& ast_builder, const Rule::HandlerArgs& args) {
         return ast_builder.HandleTakeSymbol(ast::As<ast::Symbol>(std::get<ast::NodePtr>(args[1])));
     }},
    {Identifier::SequenceOfSymbols,
     {
         Identifier::TakeSymbol,
     },
     [](AstBuilder& ast_builder, const Rule::HandlerArgs& args) {
         return ast_builder.HandleSequenceOfSymbolsBegin(
             ast::As<ast::TakeSymbol>(std::get<ast::NodePtr>(args[0])));
     }},
    {Identifier::SequenceOfSymbols,
     {
         Identifier::SequenceOfSymbols,
         Identifier::TakeSymbol,
     },
     [](AstBuilder& ast_builder, const Rule::HandlerArgs& args) {
         return ast_builder.HandleSequenceOfSymbolsContinue(
             ast::As<ast::SequenceOfSymbols>(std::get<ast::NodePtr>(args[0])),
             ast::As<ast::TakeSymbol>(std::get<ast::NodePtr>(args[1])));
     }},
    {Identifier::Production,
     {
         Identifier::SequenceOfSymbols,
         TokenType::ArrowRight,
         TokenType::Name,
     },
     [](AstBuilder& ast_builder, const Rule::HandlerArgs& args) {
         return ast_builder.HandleProduction(
             ast::As<ast::SequenceOfSymbols>(std::get<ast::NodePtr>(args[0])),
             std::get<Token>(args[2]));
     }},
    {Identifier::ProductionList,
     {
         Identifier::Production,
     },
     [](AstBuilder& ast_builder, const Rule::HandlerArgs& args) {
         return ast_builder.HandleProductionListBegin(
             ast::As<ast::Production>(std::get<ast::NodePtr>(args[0])));
     }},
    {Identifier::ProductionList,
     {
         Identifier::ProductionList,
         Identifier::Production,
     },
     [](AstBuilder& ast_builder, const Rule::HandlerArgs& args) {
         return ast_builder.HandleProductionListContinue(
             ast::As<ast::ProductionList>(std::get<ast::NodePtr>(args[0])),
             ast::As<ast::Production>(std::get<ast::NodePtr>(args[1])));
     }},
    {Identifier::RuleHeader,
     {
         TokenType::OpenSq,
         TokenType::Name,
         TokenType::CloseSq,
     },
     [](AstBuilder& ast_builder, const Rule::HandlerArgs& args) {
         return ast_builder.HandleCommonRuleHeader(std::get<Token>(args[1]));
     }},
    {Identifier::RuleHeader,
     {
         TokenType::OpenSq,
         TokenType::Name,
         TokenType::Colon,
         TokenType::Name,
         TokenType::CloseSq,
     },
     [](AstBuilder& ast_builder, const Rule::HandlerArgs& args) {
         return ast_builder.HandleTypedRuleHeader(std::get<Token>(args[1]),
                                                  std::get<Token>(args[3]));
     }},
    {Identifier::Rule,
     {
         Identifier::RuleHeader,
         Identifier::ProductionList,
     },
     [](AstBuilder& ast_builder, const Rule::HandlerArgs& args) {
         return ast_builder.HandleCommonRule(
             ast::As<ast::RuleHeader>(std::get<ast::NodePtr>(args[0])),
             ast::As<ast::ProductionList>(std::get<ast::NodePtr>(args[1])));
     }},
    {Identifier::Rule,
     {
         TokenType::At,
         Identifier::RuleHeader,
         Identifier::ProductionList,
     },
     [](AstBuilder& ast_builder, const Rule::HandlerArgs& args) {
         return ast_builder.HandleMainRule(
             ast::As<ast::RuleHeader>(std::get<ast::NodePtr>(args[1])),
             ast::As<ast::ProductionList>(std::get<ast::NodePtr>(args[2])));
     }},
    {Identifier::RuleList,
     {
         Identifier::Rule,
     },
     [](AstBuilder& ast_builder, const Rule::HandlerArgs& args) {
         return ast_builder.HandleRuleListBegin(
             ast::As<ast::Rule>(std::get<ast::NodePtr>(args[0])));
     }},
    {Identifier::RuleList,
     {
         Identifier::RuleList,
         Identifier::Rule,
     },
     [](AstBuilder& ast_builder, const Rule::HandlerArgs& args) {
         return ast_builder.HandleRuleListContinue(
             ast::As<ast::RuleList>(std::get<ast::NodePtr>(args[0])),
             ast::As<ast::Rule>(std::get<ast::NodePtr>(args[1])));
     }},
}};

const auto kToMainRule = [](ast::NodePtr node) { return ast::As<ast::RuleList>(node); };

}  // namespace pg
