#pragma once

#include <memory>

namespace pg {

class AstNode {
public:
    ~AstNode() {
    }
};

using AstNodePtr = std::shared_ptr<AstNode>;

template <typename T, typename... Args>
AstNodePtr MakeAstNode(Args&&... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}

}  // namespace pg
