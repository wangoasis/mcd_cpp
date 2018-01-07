#include "static_checker.h"
#include <cassert>

template<class To, class From>
To safe_reinterpret_cast(From from) {
    STATIC_CHECKER(sizeof(From) <= sizeof(To),
        Destination_Type_Too_Narrow);
    return reinterpret_cast<To>(from);
}

int main() {
    // double d = 5.00;
    // void *dp = &d;
    // int c = safe_reinterpret_cast<int>(dp);
    STATIC_CHECKER(1 == 0, Value_Not_Equal);

    // if use assert statement, the program would pass compilation and show error at runtime
    // the advantage of STATIC_CHECKER is it would show error at compile time
    assert(1 == 0);
    return 0;
}