#ifndef ARRAY_HPP
#define ARRAY_HPP

#include <concepts>

namespace DS {

//enum of fill behavior in array constructors
enum class FillType : char {
    fill,
    copy_n_fill,
    repeat_pattern,
    copy,
    sort
};

//special struct to make overloaded functions
template<FillType>
struct InitOptions {};

//convenient optins
//example usage: StaticArray<int, 10> arr({1,2,3}, DS::Options::RepeatPattern);
//this will create array [1,2,3,1,2,3,1,2,3,1]
namespace Options {
    constexpr auto RepeatPattern = InitOptions<FillType::repeat_pattern>{};
    constexpr auto Fill = InitOptions<FillType::fill>{};
    constexpr auto Copy_and_fill = InitOptions<FillType::copy_n_fill>{};
    constexpr auto Copy = InitOptions<FillType::copy>{};
    constexpr auto Sort = InitOptions<FillType::sort>{};
}

//static array
template<typename T = int, size_t len = 100>
struct StaticArray {

    //create array filled with val
    consteval StaticArray(const T val, InitOptions<FillType::fill> ignore = Options::Fill)
    {
        for (auto i = 0; i < len; i++)
            m_data[i] = val;
    }

    //copy values from list and rest of array is filled with garbage
    consteval StaticArray(std::initializer_list<T> lst, InitOptions<FillType::copy> ignore = Options::Copy)
    {
        auto i = 0;
        for (auto it = lst.begin(); it != lst.end(); it++)
            m_data[i++] = *it;
    }

    //copy values from list and fill rest of array with desired values
    consteval StaticArray(const T val, std::initializer_list<T> lst, InitOptions<FillType::copy_n_fill> ignore = Options::Copy_and_fill)
    {
        auto i = 0;
        for (auto it = lst.begin(); it != lst.end(); it++)
            m_data[i++] = *it;

        for (;i < len;)
            m_data[i++] = val;
    }

    //copy values from list untill array filled with pattern
    consteval StaticArray(std::initializer_list<T> lst, InitOptions<FillType::repeat_pattern>)
    {
        auto i = 0;
        for (auto it = lst.begin(); i < len; it++){
            if (it == lst.end()){
                it = lst.begin();
            }

            m_data[i++] = *it;
        }
    }

    //sort values then copy them
    consteval StaticArray(std::initializer_list<T> lst, InitOptions<FillType::sort>)
    {
        auto i = 0;
        for (auto it = lst.begin(); it != lst.end(); it++)
            m_data[i++] = *it;
        std::sort(std::begin(m_data), std::end(m_data));
    }

    //dont need custom iterators since class is a wrapper over basic array
    auto begin(){ return std::ranges::begin(m_data); }
    auto cbegin() const { return std::ranges::cbegin(m_data); }
    auto end(){ return std::ranges::end(m_data); }
    auto cend(){ return std::ranges::cend(m_data); }

    T& operator[](size_t index){ return m_data[index]; }

    constexpr auto size() const { return len; }

protected:

    T m_data[len]{{}};
};

} //DS namespace

#endif //ARRAY_HPP