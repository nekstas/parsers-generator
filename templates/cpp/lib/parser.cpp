#include "parser.h"

#include "../data/grammar.h"
#include "../data/tables.h"

pg::LrParser::LrParser(const pg::LrData& data) : data_(data) {
}

void pg::LrParser::Parse(const pg::Tokenizer::Result& input, pg::AstBuilder& ast_builder) {
    const auto& tokens = input.tokens;

    if (tokens.empty() || tokens.back().type != TokenType::Eof) {
        throw std::logic_error{"The parser received an incorrect list of tokens."};
    }

    std::vector<size_t> states = {0};
    std::vector<Rule::HandlerArg> result_stack;
    for (size_t i = 0; i < tokens.size() && !states.empty();) {
        size_t current_state = states.back();
        Token current_token = tokens.at(i);

        LrAction action = data_.action_table.at(current_state).at(current_token.type);
        if (action.type == LrAction::Type::SHIFT) {
            states.push_back(action.index);
            result_stack.push_back(current_token);
            ++i;
        } else if (action.type == LrAction::Type::REDUCE) {
            const Rule& rule = data_.grammar.rules.at(action.index);
            size_t k = rule.sequence.size();
            Rule::HandlerArgs args(result_stack.end() - k, result_stack.end());
            for (size_t j = 0; j < k; ++j) {
                states.pop_back();
                result_stack.pop_back();
            }
            states.push_back(data_.goto_table.at(states.back()).at(rule.result));
            result_stack.push_back(rule.handler(ast_builder, args));
        } else {
            if (action.type == LrAction::Type::ACCEPT) {
                ast_builder.Accept(std::get<ast::AstNodePtr>(result_stack.back()));
            } else {
                ast_builder.Error(input, i);
            }
            break;
        }
    }
}

pg::LrParser pg::LrParser::Create() {
    return pg::LrParser({kGrammar, kActionTable, kGotoTable});
}
