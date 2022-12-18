#include <memory>
#define DS_DEBUG_LIST
#include <structarnica/dlist.hpp>
#include <cassert>
#include <iostream>

using namespace std;

#define SList DList

void test_constructors() {
    std::cout << "test_constructors()\n";

    //empty constructor
    DS::SList<int> ints;

    assert(ints.empty());
    assert(ints.size() == 0);
    assert(!ints.first().has_value());
    assert(!ints.last().has_value());
    ints.print_list();

    //init with 10 default values
    DS::SList<int> ints2(10);

    assert(!ints2.empty());
    assert(ints2.size() == 10);
    ints2.print_list();

    //init from std::initializer_list
    DS::SList<int> ints3{1,2,3,4,5};

    assert(!ints3.empty());
    assert(ints3.size() == 5);
    ints3.print_list();

    //init from other list
    DS::SList<int> ints4(ints3);

    assert(!ints4.empty());
    assert(ints4.size() == ints3.size());
    ints4.print_list();

    //init from other moved list
    DS::SList<int> ints5(DS::SList<int>{10,20,30,40,50});
    assert(!ints5.empty());
    assert(ints5.size() == 5);
    ints5.print_list();

    //copy assignment
    DS::SList<int> ints6 = ints5;
    assert(!ints6.empty());
    assert(ints6.size() == 5);
    ints6.print_list();

    DS::SList<int> ints7 = DS::SList<int>(20);
    assert(!ints7.empty());
    assert(ints7.size() == 20);
    ints7.print_list();
}

void test_member_functions() {
    std::cout << "test_member_functions()\n";

    DS::SList<int> plist;

    plist.push_back(1);

    assert(plist.front() == 1);
    assert(plist.size() == 1);

    plist.push_back(2);

    assert(plist.back() == 2);
    assert(plist.size() == 2);

    plist.push_back(3);

    assert(plist.front() == 1);
    assert(plist.back() == 3);
    assert(plist.size() == 3);

    plist.print_list();

    DS::SList<int> flist;

    flist.push_front(5);

    assert(flist.front() == 5);
    assert(flist.back() == 5);
    assert(flist.size() == 1);

    flist.push_front(4);

    assert(flist.front() == 4);
    assert(flist.back() == 5);
    assert(flist.size() == 2);

    flist.push_front(3);

    assert(flist.front() == 3);
    assert(flist.back() == 5);
    assert(flist.size() == 3);

    flist.print_list();

    DS::SList<int> lst{1,2,3};

    assert(lst.size() == 3);
    assert(lst.first().value() == 1);
    assert(lst.last().value() == 3);

    lst.clear();

    assert(lst.size() == 0);

    lst = DS::SList<int>{4,3,2,1};

    lst.print_list();

    assert(lst.first().value() == 4);
    assert(lst.last().value() == 1);

    auto o1 = lst.find(2);

    assert(o1 != lst.end());

    assert(lst.cfront() == lst.front() && lst.first().value() == lst.front() && lst.front() == 4);
    assert(lst.back() == lst.cback() && lst.last().value() == lst.back() && lst.back() == 1);

    lst.push_back(123);
    assert(lst.back() == 123);

    lst.push_front(929292);
    assert(lst.front() == 929292);

    assert(lst.pop_back() == 123);
    assert(lst.back() == 1);

    assert(lst.pop_front() == 929292);
    assert(lst.front() == 4);

    auto lst2 = DS::SList<int>(10);

    lst.print_list();

    assert(lst2.count(0) == 10);
    assert(lst.count(0) == 0);
}

void test_erase() {
    std::cout << "test_erase()\n";

    DS::SList<int> lst;

    assert(!lst.erase(123));

    lst = DS::SList<int>({1});

    assert(!lst.erase(123));
    assert(lst.erase(1));
    assert(lst.size() == 0);

    lst = DS::SList<int>{1,2,3,4,5};

    lst.print_list();

    assert(!lst.erase(123));

    lst.print_list();

    assert(lst.erase(1));

    lst.print_list();

    assert(lst.size() == 4);

    lst.print_list();

    assert(lst.erase(4));

    lst.print_list();
}

//basic testing of all functions
void test_operators() {
    cout << "test_operators()\n";
    DS::SList<int> lst;

    assert(!lst);

    lst = DS::SList<int>{1,2,3,4};

    assert(lst);

    auto lst2 = lst + DS::SList<int>{8,8,8,8};
    assert(!(lst == lst2));
    assert(lst != lst2);

    lst2.print_list();

    assert(lst2.size() == 8);

    lst2 += DS::SList<int>{9,9,9};

    lst2.print_list();

    assert(lst2.size() == 11);

    auto lst3 = DS::SList<int>{1,2,3,4};

    assert(lst3 == lst);
    assert(lst3 != lst2);
}

void test_iterators() {
    cout << "test_iterators()\n";
    DS::SList<int> lst{1,2,3,4,5,6,7};

    for (const auto& x : lst)
        std::cout << x << '\n';

    auto it = std::find(lst.begin(), lst.end(), 3);

    std::cout << *it << '\n';

    lst.erase(it);

    lst.print_list();

    lst = DS::SList<int>{1,2,3,4,5,6,7,8,9};

    it = std::find(lst.begin(), lst.end(), 4);

    lst.erase(it, lst.end());

    lst.print_list();
}

int main(){

    test_constructors();
    test_member_functions();
    test_erase();
    test_iterators();
    test_operators();

    return 0;
}
