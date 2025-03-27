#pragma once

#include <iostream>
#include <set>

template <typename T>
std::ostream& operator<<(std::ostream& out, const std::set<T>& values) {
    out << "{";
    for (const T& value : values) {
        if (&value != &*values.begin()) {
            out << ", ";
        }
        out << value;
    }
    return out << "}";
}
