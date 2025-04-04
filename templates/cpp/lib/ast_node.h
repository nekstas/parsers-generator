#pragma once

#include <memory>

namespace ast {

class AstNode {
public:
    virtual ~AstNode() {
    }
};

using AstNodePtr = std::shared_ptr<AstNode>;

template <typename T, typename... Args>
std::shared_ptr<T> Make(Args&&... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}

template <typename T>
std::shared_ptr<T> As(AstNodePtr node) {
    return std::dynamic_pointer_cast<T>(node);
}

}  // namespace ast
