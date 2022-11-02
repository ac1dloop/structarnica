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
#include <string_view>
#include <chrono>

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

//print name of a type
//copied from stackoverflow
template <typename T>
constexpr auto type_name() {
  std::string_view name, prefix, suffix;
#ifdef __clang__
  name = __PRETTY_FUNCTION__;
  prefix = "auto type_name() [T = ";
  suffix = "]";
#elif defined(__GNUC__)
  name = __PRETTY_FUNCTION__;
  prefix = "constexpr auto type_name() [with T = ";
  suffix = "]";
#elif defined(_MSC_VER)
  name = __FUNCSIG__;
  prefix = "auto __cdecl type_name<";
  suffix = ">(void)";
#endif
  name.remove_prefix(prefix.size());
  name.remove_suffix(suffix.size());
  return name;
}

struct Timer {

    Timer():m_clock{std::chrono::high_resolution_clock::now()}
    {}

    void reset(){
        m_clock = std::chrono::high_resolution_clock::now();
    }

    auto time(){
        return std::chrono::high_resolution_clock::now() - m_clock;
    }

    template<typename T>
    auto time_as(){
        return std::chrono::duration_cast<T>(time());
    }

protected:

    decltype(std::chrono::high_resolution_clock::now()) m_clock;

};

} //DS namespace

#endif //UTIL_HPP