#include "parser.h"

#include <iostream>

pg::LrParser::LrParser(const pg::LrData& data) : data_(data) {
}

void pg::LrParser::Parse(const std::vector<Token>& tokens) {
    if (tokens.empty() || tokens.back().GetTokenType() != TokenType::Eof) {
        // TODO: throw an error
        throw std::logic_error{"Incorrect sequence of tokens from tokenizer"};
    }

    std::vector<size_t> states = {0};
    for (size_t i = 0; i < tokens.size() && !states.empty();) {
        size_t current_state = states.back();
        Token current_token = tokens.at(i);

        LrAction action = data_.action_table.at(current_state).at(current_token.GetTokenType());
        if (action.type == LrAction::Type::SHIFT) {
            states.push_back(action.index);
            ++i;
        } else if (action.type == LrAction::Type::REDUCE) {
            const Rule& rule = data_.grammar.rules.at(action.index);
            for (size_t j = 0; j < rule.sequence.size(); ++j) {
                states.pop_back();
            }
            states.push_back(data_.goto_table.at(states.back()).at(rule.result));
            // Todo call process function
            std::cerr << "Reduce " << data_.grammar.rules.at(action.index).repr << "\n";
        } else if (action.type == LrAction::Type::ACCEPT) {
            // Todo accept action
            break;
        } else {
            // Todo error action
            throw std::runtime_error{"Error while parsing input."};
        }
    }
}
