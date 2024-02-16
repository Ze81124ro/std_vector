#include <iostream>
#include "vector.hpp"

int main() {
    Vector<int> v = {1, 2, 3, 4, 5};
    v.push_back(6);
    v.push_back(7);
    for (int i : v)
        std::cout << i << "\n";
    std::cout << "Successfuly!!!\n";
}
