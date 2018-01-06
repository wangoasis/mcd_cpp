#ifndef _STATIC_CHECKER_H
#define _STATIC_CHECKER_H

namespace Mcd {

    template<int> struct CompileTimeError;
    template<> struct CompileTimeError<true> {};
}

/*  
    if expr is false, then it would initialize a class from CompileTimeError<false> template, 
    which is not defined. The error would show on compilation time.
    Notice the use of msg parameter, ERROR_##msg would concatenate ERROR_ and the content
    of msg parameter.
*/
#define STATIC_CHECKER(expr, msg) \
{ Mcd::CompileTimeError<((expr) != 0)> ERROR_##msg; (void)ERROR_##msg; } 

#endif 
