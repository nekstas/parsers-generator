cmake_minimum_required(VERSION 3.29)
project(parsers_generator)

set(CMAKE_CXX_STANDARD 23)

add_custom_command(
        OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/template/lexer.cpp
        COMMAND re2c ${CMAKE_CURRENT_SOURCE_DIR}/template/lexer.re -i -o ${CMAKE_CURRENT_SOURCE_DIR}/template/lexer.cpp
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/template/lexer.re
        VERBATIM)

add_executable(parsers_generator main.cpp
        template/lexer.h
        template/lexer.cpp
        template/tokenizer.cpp
        template/tokenizer.h
        core/grammar/grammar.cpp
        core/grammar/grammar.h
        core/grammar/grammar_info.cpp
        core/grammar/grammar_info.h
        utils/input_output.hpp
        core/generators/slr_generator.cpp
        core/generators/slr_generator.h
)
