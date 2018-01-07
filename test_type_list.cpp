#include "type_list.h"
#include <iostream>
#include <cassert>
using std::cout;
using std::endl;

class AGrandParent {
public: 
    AGrandParent() {}
};

class AParent : public AGrandParent {
public: 
    AParent() {}
};

class A : public AParent{
public: 
    A() {}
};


class ASon : public A {
public: 
    ASon() {}
};

class AGrandSon : public ASon {
public: 
    AGrandSon() {}
};

int main() {
    typedef TYPELIST_5(A, AParent, AGrandParent, ASon, AGrandSon) test_list;
    assert((Mcd::TL::Length<test_list>::value) == 5);
    assert((Mcd::TL::IndexOf<test_list, ASon>::value) == 3);

    typedef Mcd::TL::Append<test_list, ASon>::Result test_list_2;
    assert((Mcd::TL::Length<test_list_2>::value) == 6);

    typedef Mcd::TL::NoDuplicates<test_list_2>::Result test_list_3;
    assert((Mcd::TL::Length<test_list_3>::value) == 5);

    typedef Mcd::TL::DerivedToFront<test_list_3>::Result test_list_4;
    // order should be <GrandSon, son, A, parent, grandparent>
    assert((Mcd::TL::IndexOf<test_list_4, AGrandSon>::value) == 0);
    assert((Mcd::TL::IndexOf<test_list_4, ASon>::value) == 1);
    assert((Mcd::TL::IndexOf<test_list_4, A>::value) == 2);
    assert((Mcd::TL::IndexOf<test_list_4, AParent>::value) == 3);
    assert((Mcd::TL::IndexOf<test_list_4, AGrandParent>::value) == 4);

    return 0;
}