// classical_search.h
#pragma once
#include <vector>
#include <optional>
#include <algorithm>
#include <cstddef>

template <typename T>
std::optional<std::size_t> linear_search(const std::vector<T>& a, const T& key) {
    for (std::size_t i = 0; i < a.size(); ++i)
        if (a[i] == key) return i;
    return std::nullopt;
}

template <typename T>
std::optional<std::size_t> binary_search_index(const std::vector<T>& a, const T& key) {
    std::size_t lo = 0, hi = a.size();
    while (lo < hi) {
        std::size_t mid = lo + (hi - lo) / 2;
        if (a[mid] < key) lo = mid + 1;
        else hi = mid;
    }
    if (lo < a.size() && !(a[lo] < key) && !(key < a[lo])) return lo;
    return std::nullopt;
}
