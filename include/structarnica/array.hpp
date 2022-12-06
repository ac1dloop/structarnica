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
template<typename T = int, size_t Length = 100, size_t BaseIndex = 0>
struct StaticArray {

    static_assert(BaseIndex == 0 || BaseIndex == 1, "Base index more than 1 is not allowed");

    //create array filled with val
    consteval StaticArray(const T val = {}, InitOptions<FillType::fill> Val = Options::Fill) {
        for (auto i = 0; i < Length; i++)
            m_data[i] = val;
    }

    //copy values from list and rest of array is filled with garbage
    consteval StaticArray(std::initializer_list<T> lst, InitOptions<FillType::copy> Val = Options::Copy) {
        auto i = 0;
        for (auto it = lst.begin(); it != lst.end(); it++)
            m_data[i++] = *it;
    }

    //copy values from list and fill rest of array with desired values
    consteval StaticArray(const T val, std::initializer_list<T> lst, InitOptions<FillType::copy_n_fill> Val = Options::Copy_and_fill) {
        auto i = 0;
        for (auto it = lst.begin(); it != lst.end(); it++)
            m_data[i++] = *it;

        for (;i < Length;)
            m_data[i++] = val;
    }

    //copy values from list untill array filled with pattern
    consteval StaticArray(std::initializer_list<T> lst, InitOptions<FillType::repeat_pattern>) {
        auto i = 0;
        for (auto it = lst.begin(); i < Length; it++){
            if (it == lst.end()){
                it = lst.begin();
            }

            m_data[i++] = *it;
        }
    }

    //sort values then copy them
    consteval StaticArray(std::initializer_list<T> lst, InitOptions<FillType::sort>) {
        auto i = 0;
        for (auto it = lst.begin(); it != lst.end(); it++)
            m_data[i++] = *it;
        std::sort(std::begin(m_data), std::end(m_data));
    }

    //dont need custom iterators since class is a wrapper over basic array
    auto begin(){ return std::ranges::begin(m_data); }
    auto cbegin() const { return std::ranges::cbegin(m_data); }
    auto end(){ return std::ranges::end(m_data); }
    auto cend() const { return std::ranges::cend(m_data); }

    template<std::unsigned_integral IndexType>
    constexpr T& operator[](IndexType index) {
        return m_data[index - BaseIndex]; 
    }

    template<std::signed_integral IndexType>
    constexpr T& operator[](IndexType index) {
        return index >= 0 ? m_data[index - BaseIndex] : m_data[Length + index];
    }

    //this makes all templated classes StaticArray friends :)
    template<typename, size_t, size_t>
    friend struct StaticArray;

    template<size_t Start, size_t End>
    constexpr auto slice() const {
        StaticArray<T, (End - Start), BaseIndex> res;

        for (auto i = 0; i < res.size(); ++i){
            res.m_data[i] = m_data[i];
        }

        return res;
    }

    consteval auto size() const { return Length; }

protected:

    T m_data[Length]{{}};
};

} //DS namespace

#endif //ARRAY_HPP