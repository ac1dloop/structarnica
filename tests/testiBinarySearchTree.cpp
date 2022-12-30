#include <structarnica/bst.hpp>
#include <random>
#include <functional>
#include <vector>
#include <iostream>

using namespace std;
using namespace DS;

template<typename T>
struct Comp {
    bool operator()(T a, T b){
        return a < b;
    }
};

int main() {

//    auto rnd = std::bind(std::uniform_int_distribution<int>(1, 999), std::mt19937());

    std::vector<int> vec{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};

    BST<int, Comp<int> > tree1, tree2;

    for (auto x : vec){
        tree1.insert(x);
        tree2.insert(x);
    }

    auto printer = [](int a){ cout << a << ' '; };
    std::cout << "before transform\n";
    tree1.preorder(printer);
    std::cout << '\n';
    std::cout << "tree1 len: " << tree1.transform_to_list() << '\n';
    std::cout << "after transform\n";
    tree1.preorder(printer);
    std::cout << '\n';

    tree1.balance();

    std::cout << "after balance\n";
    tree1.preorder(printer);
    std::cout << '\n';

    return 0;
}
