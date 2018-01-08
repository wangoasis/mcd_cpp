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
}