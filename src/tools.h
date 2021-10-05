#pragma once

#include <algorithm>
#include <array>

namespace tool {

template <typename T, std::size_t N>
constexpr std::array<T, N> Sort(std::array<T, N> array) {
    std::sort(array.begin(), array.end());
    return array;
}

} // namespace tool
