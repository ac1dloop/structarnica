#include <iostream>
#include <iomanip>
#include <algorithm>
#include <source_location>
#include <concepts>
#include <ranges>
#include <vector>
#include <array.hpp>
#include <util.hpp>

using namespace DS;
int main() 
{
    DS::StaticArray<int, 200> arr({1,2,0,5,1,6,7,12,33}, DS::Options::Sort);

    std::cout << arr << std::endl;

	return 0;
}