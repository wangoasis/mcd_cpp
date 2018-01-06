#include "static_checker.h"

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
    return 0;
}