/**
 * @brief utilities
 * @file util.hpp
 * 
 * concepts, utilities for printing, testing
 * basically anything that is not data structure
 */
#ifndef UTIL_HPP
#define UTIL_HPP

#include <cstdint>
#include <ranges>
#include <concepts>
#include <iostream>

namespace DS {

//compile time sum of basic array
template <typename T, size_t Size>
constexpr int32_t const_sum(const T arr[Size])
{
    int32_t ret = 0;
    for (auto i = 0; i < Size; ++i)
        ret += arr[i];
    return ret;
}

/**
 * @brief print arrays
 * 
 * overload of 'operator<<' for ranges/arrays
 * 
 * @tparam T 
 * @param os std stream
 * @param arr any range
 * @return std::ostream&
 */
template<std::ranges::range T>
requires std::ranges::range<T>
std::ostream& operator<<(std::ostream& os, T arr){

    unsigned i = 1;
    std::ranges::for_each(arr, [&](auto d){
        os << d << (i++ % 20 == 0 ? '\n' : ' ');
    });

    return os;
}


} //DS namespace

#endif //UTIL_HPP