#ifndef _STATIC_CHECKER_H
#define _STATIC_CHECKER_H

namespace Mcd {

    template<int> struct CompileTimeError;
    template<> struct CompileTimeError<true> {};
}

#define STATIC_CHECKER(expr, msg) \
{ Mcd::CompileTimeError<((expr) != 0)> ERROR_##msg; (void)ERROR_##msg; } 

#endif 
