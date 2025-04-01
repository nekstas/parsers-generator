cmake_minimum_required(VERSION 3.29)
project(parsers_generator)

set(CMAKE_CXX_STANDARD 23)

add_custom_command(
        OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/template/usr/lexer.cpp
        COMMAND re2c ${CMAKE_CURRENT_SOURCE_DIR}/template/usr/lexer.re -i -o ${CMAKE_CURRENT_SOURCE_DIR}/template/usr/lexer.cpp
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/template/usr/lexer.re
        VERBATIM)

add_executable(parsers_generator main.cpp
        template/usr/lexer.h
        template/usr/lexer.cpp
        template/lib/tokenizer.cpp
        template/lib/tokenizer.h
        core/grammar/grammar.cpp
        core/grammar/grammar.h
        core/grammar/grammar_info.cpp
        core/grammar/grammar_info.h
        utils/input_output.hpp
        core/generators/slr_generator.cpp
        core/generators/slr_generator.h
        template/lib/parser.cpp
        template/lib/parser.h
        template/lib/lr_data.h
        template/lib/grammar.h
        template/data/identifier.h
        core/code/cpp_generator.cpp
        core/code/cpp_generator.h
        template/data/tables.h
        template/data/grammar.h
        template/data/ast_builder.h
        template/lib/ast_node.h
        template/data/token_type.h
        template/lib/token.h
)
