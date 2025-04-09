#include "filesystem.h"

#include <fstream>

#include "../core/app/errors.h"
#include "format_stream.h"

std::string fs::ReadFile(const std::string& file_path) {
    std::ifstream file(file_path);
    if (!file.is_open()) {
        throw errors::FilesystemError{FormatStream()
                                      << "Can't read a file \"" << file_path << "\"."};
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}
