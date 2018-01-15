#include "small_obj.h"
#include <iostream>
#include <cassert>

int main() {
    Mcd::SmallObject<> test_obj; 

    int* p = static_cast<int*>(test_obj.newMemory(45));
    *p = 5;
    assert(p != nullptr);
    std::cout << p << std::endl;
    std::cout << *p << std::endl;

    test_obj.deleteMemory(p, 45);
    std::cout << p << std::endl;
    std::cout << *p << std::endl;

    std::cout << "test small_obj finished " << std::endl;
    return 0;
}