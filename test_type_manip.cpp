#include "type_manip.h"
#include <iostream>

class Base {
public:
    Base() {} ;
};

class Derived : public Base {
public:
    Derived() {} ;
};

int main() {
    Base b;
    Derived d;
    std::cout << SUPER_SUBCLASS(Base, Derived) << std::endl; // true
    std::cout << SUPER_SUBCLASS(Derived, Base) << std::endl; // false
    std::cout << SUPER_SUBCLASS(Derived, Derived) << std::endl;// true
    std::cout << SUPER_SUBCLASS(Derived, void) << std::endl; // false
    std::cout << std::endl;
    std::cout << SUPER_SUBCLASS_STRICT(Base, Derived) << std::endl;// true
    std::cout << SUPER_SUBCLASS_STRICT(Derived, Base) << std::endl; // false
    std::cout << SUPER_SUBCLASS_STRICT(Derived, Derived) << std::endl; // false
    std::cout << SUPER_SUBCLASS_STRICT(Base, void) << std::endl; // false
    std::cout << std::endl;
    std::cout << IS_SAME_TYPE(Base, Base) << std::endl;// true
    std::cout << IS_SAME_TYPE(Derived, Derived) << std::endl;// true
    std::cout << IS_SAME_TYPE(Base, Derived) << std::endl; // false
}