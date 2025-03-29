#pragma once

#include <iostream>
#include <map>
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

template <typename K, typename V>
std::ostream& operator<<(std::ostream& out, const std::map<K, V>& values) {
    for (const auto& pair : values) {
        out << pair.first << ": " << pair.second << "\n";
    }
    return out;
}
