#pragma once

#include <sstream>

class FormatStream : public std::stringstream {
public:
    operator std::string() const {
        return str();
    }
};
