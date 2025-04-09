#pragma once

#include <memory>
#include <stdexcept>

namespace ast {

class Node {
public:
    virtual ~Node() {
    }
};

using NodePtr = std::shared_ptr<Node>;

template <typename T, typename... Args>
std::shared_ptr<T> Make(Args&&... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}

template <typename T>
std::shared_ptr<T> As(NodePtr node) {
    std::shared_ptr<T> result = std::dynamic_pointer_cast<T>(node);
    if (!result) {
        throw std::logic_error{"Can't cast to given node type."};
    }
    return std::dynamic_pointer_cast<T>(node);
}

}  // namespace ast