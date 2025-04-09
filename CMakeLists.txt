cmake_minimum_required(VERSION 3.29)
project(parsers_generator)

set(CMAKE_CXX_STANDARD 23)

#add_custom_command(
#        OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/core/grammar_parser/usr/lexer.cpp
#        COMMAND re2c ${CMAKE_CURRENT_SOURCE_DIR}/core/grammar_parser/usr/lexer.re -i -o ${CMAKE_CURRENT_SOURCE_DIR}/core/grammar_parser/usr/lexer.cpp
#        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/core/grammar_parser/usr/lexer.re
#        VERBATIM)

add_executable(parsers_generator main.cpp
        core/grammar/grammar.cpp
        core/grammar/grammar.h
        core/grammar/grammar_info.cpp
        core/grammar/grammar_info.h
        utils/input_output.hpp
        core/generators/slr_generator.cpp
        core/generators/slr_generator.h
        core/code/cpp_generator.cpp
        core/code/cpp_generator.h
        core/app/application.h
        core/app/application.cpp
        core/code/code_generator.h
        core/code/filesystem_helper.cpp
        core/code/filesystem_helper.h
        core/app/errors.h
        utils/filesystem.cpp
        utils/filesystem.h
        core/grammar_parser/lib/parser.cpp
        core/grammar_parser/lib/tokenizer.cpp
        core/grammar_parser/lib/error_printer.cpp
        core/grammar_parser/usr/lexer.cpp
        core/grammar_parser/grammar_parser.cpp
        core/grammar_parser/grammar_parser.h
        core/grammar_parser/usr/grammar_builder.cpp
        core/grammar_parser/usr/grammar_builder.h
        core/grammar_parser/usr/grammar_builder.h
)
