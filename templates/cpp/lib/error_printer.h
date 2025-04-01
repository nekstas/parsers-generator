#pragma once

#include "tokenizer.h"

namespace pg {

class ErrorPrinter {
public:
    static void UnexpectedToken(const Tokenizer::Result& input, size_t error_index);
};

}  // namespace pg
