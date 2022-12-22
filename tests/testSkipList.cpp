#include <iostream>
#include <iomanip>
#include <algorithm>
#include <source_location>
#include <concepts>
#include <ranges>
#include <vector>
#include <random>
#include <functional>
#include <structarnica/skip_list.hpp>

using namespace std;
using namespace DS;

int main() {

    auto rnd = std::bind(std::uniform_int_distribution<int>(1, 999), std::mt19937());

    SkipList<int> lst;

    lst.insert(rnd());
    lst.insert(rnd());
    lst.insert(rnd());
    lst.insert(rnd());
    lst.insert(rnd());
    lst.insert(rnd());

    lst.insert(3);
    lst.insert(2);
    lst.insert(1);

    lst.insert(1000);
    lst.insert(905121);
    lst.insert(0);

    cout << lst.dump();

    auto it = lst.find(1000);

    if (it){
        cout << "value: " << *it << '\n';
    }

    lst.remove(1000);

    cout << lst.dump();

    return 0;
}
