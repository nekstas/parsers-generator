#pragma once

#include <stdexcept>

namespace errors {

class FilesystemError : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

class ApplicationError : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

}  // namespace errors
