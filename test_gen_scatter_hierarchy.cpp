#include "hierarchy_generators.h"
#include "type_list.h"
#include <string>
#include <vector>
#include <cassert>
using std::vector;
using std::string;

template <typename T>
struct Holder {
    T value_;
};

int main() {
    typedef Mcd::GenScatterHierarchy<TYPELIST_3(int, string, vector<int>), Holder> test_H;
    test_H obj;
    Mcd::Field<int>(obj).value_ = 5;
    Mcd::Field<string>(obj).value_ = "test_string";
    Mcd::Field<vector<int>>(obj).value_ = {1,2,3} ;

    assert(Mcd::Field<int>(obj).value_ == 5);
    assert(Mcd::Field<string>(obj).value_ == "test_string");
    assert(Mcd::Field<vector<int>>(obj).value_[0] == 1) ;
    /*
        Try to access a type not int a Typelist would cause compilation time error
    */
    // assert(Mcd::Field<double>(obj).value_ == 1);

    /*
        test Tuple
        Tuple does not need to specify Holder type, it's already defined in header file
        Tuple does not need .value_ to access the value_, the operator () has been overloaded
    */
    typedef Mcd::Tuple<TYPELIST_3(int, int, int)> Point3D;
    Point3D p;
    Mcd::Field<0>(p) = 1;
    Mcd::Field<1>(p) = 5;
    Mcd::Field<2>(p) = 2;

    assert(Mcd::Field<0>(p) == 1);
    assert(Mcd::Field<1>(p) == 5);
    assert(Mcd::Field<2>(p) == 2);
}