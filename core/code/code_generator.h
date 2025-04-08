#pragma once

#include <string>

namespace code {

class CodeGenerator {
public:
    virtual ~CodeGenerator() {
    }

    virtual void Create(const std::string& path) = 0;
};

}  // namespace code
