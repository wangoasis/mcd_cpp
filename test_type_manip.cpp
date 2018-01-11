#include "type_manip.h"
#include <cassert>

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
    assert(SUPER_SUBCLASS(Base, Derived) == 1); // true
    assert(SUPER_SUBCLASS(Derived, Base) == 0); // false
    assert(SUPER_SUBCLASS(Derived, Derived) == 1);// true
    assert(SUPER_SUBCLASS(Derived, void) == 0);  // false
    assert(SUPER_SUBCLASS_STRICT(Base, Derived) == 1);// true
    assert(SUPER_SUBCLASS_STRICT(Derived, Base) == 0); // false
    assert(SUPER_SUBCLASS_STRICT(Derived, Derived) == 0); // false
    assert(SUPER_SUBCLASS_STRICT(Base, void) == 0); // false
    assert(IS_SAME_TYPE(Base, Base) == 1); // true
    assert(IS_SAME_TYPE(Derived, Derived) == 1);// true
    assert(IS_SAME_TYPE(Base, Derived) == 0); // false
}
