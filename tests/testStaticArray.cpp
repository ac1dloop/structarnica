#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <structarnica/array.hpp>
#include <cassert>

using namespace std;
using namespace DS;

int main(){

    //create array filled with default values
    StaticArray<double, 50> arr;

    cout << "StaticArray<double, 50> arr;\n";
    for (auto x : arr){
        cout << x << ' ';
        assert(x == 0.0);
    }
    cout << endl;

    //create array filled with custom value
    StaticArray<int, 50> arr1(1);

    cout << "StaticArray<int, 50> arr1(1);\n";
    for (auto x : arr1){
        cout << x << ' ';
        assert(x == 1);
    }
    cout << '\n';

    using Arr = StaticArray<int, 20>;

    //create array filled with values and leave rest with default
    Arr arr2({1,2,3,4,5,6,7,8,9,10});

    for (int i = 0; i < 10; i++){
        assert(arr2[i] == i + 1);
    }

    for (int i = 10; i < arr2.size(); i++){
        assert(arr2[i] == 0);
    }

    cout << "Arr arr2({1,2,3,4,5,6,7,8,9,10});\n";
    for (auto x : arr2){
        cout << x << ' ';
    }
    cout << '\n';

    //create array '1,2,3,4,5' and the rest is -20
    Arr arr3(-20, {1,2,3,4,5});

    for (int i = 0; i < 5; i++){
        assert(arr3[i] == i + 1);
    }

    for (int i = 5; i < arr3.size(); i++){
        assert(arr3[i] == -20);
    }

    cout << "Arr arr3(-20, {1,2,3,4,5});\n";
    for (auto x : arr3)
        cout << x << ' ';
    cout << '\n';

    //create array with repeated pattern
    Arr arr4({1,3,5}, Options::RepeatPattern);

    assert(arr4[0] == 1);
    assert(arr4[1] == 3);
    assert(arr4[2] == 5);

    assert(arr4[3] == 1);
    assert(arr4[4] == 3);
    assert(arr4[5] == 5);

    cout << "Arr arr4({1,3,5}, Options::RepeatPattern);\n";
    for (auto x : arr4)
        cout << x << ' ';
    cout << '\n';

    //create sorted array
    Arr sorted_arr({7,-1,2,12,4,5,8,0,10}, Options::Sort);

    assert(sorted_arr[0] == -1);
    assert(sorted_arr[sorted_arr.size() - 1] == 12);

    cout << "Arr sorted_arr({7,-1,2,12,4,5,8,0,10}, Options::Sort);\n";
    for (auto x : sorted_arr)
        cout << x << ' ';
    cout << '\n';

    //arr with indexing starting from 1. hello matlab chads
    StaticArray<int, 10, 1> arr5({1,2,3,4,5,6,7,8,9,10});

    assert(arr5[1] == 1);
    assert(arr5[10] == 10);

    cout << "StaticArray<int, 10, 1> arr5({1,2,3,4,5,6,7,8,9,10});\n";
    cout << arr5[1] << '\n';
    cout << arr5[2] << '\n';

    //can be indexed from the back
    StaticArray<int, 5> arr6({1,2,3,4,5});

    assert(arr6[-1] == 5);
    assert(arr6[-2] == 4);
    assert(arr6[-3] == 3);
    assert(arr6[0] == arr6[-5]);

    cout << "StaticArray<int, 5> arr6({1,2,3,4,5});\n";
    cout << arr6[-1] << '\n';
    cout << arr6[-3] << '\n';
    cout << arr6[2] << '\n';

    //create slice from array [Start, End)
    auto sliced = arr6.slice<0, 3>();

    assert(sliced[0] == 1);
    assert(sliced[1] == 2);
    assert(sliced[2] == 3);

    cout << "auto sliced = arr6.slice<0, 3>();\n";
    for (auto x : sliced)
        cout << x << ' ';
    cout << '\n';

    return 0;
}