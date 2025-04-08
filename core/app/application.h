#pragma once

#include <cstddef>
#include <cstdint>
#include <string>

class Application {
private:
    static constexpr size_t kRequiredArgsCount = 4;

public:
    Application() = default;

    void PrintHelpMessage();

    void GenerateParser(const std::string& action, const std::string& grammar_file,
                        const std::string& output_path);

    int32_t Run(size_t argc, char** argv);
};
