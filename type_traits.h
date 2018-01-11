#ifndef _TYPE_TRAITS_H
#define _TYPE_TRAITS_H

#include "null_type.h"

namespace Mcd {

    template <typename T> 
    class TypeTraits {
    private:
        /*
            check pointer
        */
        template <typename U>
        struct PointerTraits {
            enum { result = false };
            typedef NullType PointeeType;
        };

        template <typename U>
        struct PointerTraits<U*> {
            enum { result = true };
            typedef U PointeeType;
        };

        /*
            check reference
        */
        template <class U> struct ReferenceTraits {
            enum { result = false };
            typedef U ReferredType;
        };
        
        template <class U> struct ReferenceTraits<U&> {
            enum { result = true };
            typedef U ReferredType;
        };

        /*
            check const
        */
        template <class U> struct ConstTraits {
            typedef U ResultType;
            enum { result = 0 };
        };
        
        template <class U> struct ConstTraits<const U> {
            typedef U ResultType;
            enum { result = 1 };
        };

    public:
        enum { isPointer = PointerTraits<T>::result };
        typedef typename PointerTraits<T>::PointeeType PointeeType;

        enum { isReference = ReferenceTraits<T>::result };
        typedef typename ReferenceTraits<T>::ReferredType ReferredType;

        enum { isConst = ConstTraits<T>::result };
        typedef typename ConstTraits<T>::ResultType NonConstType;
    };
} // namespace Mcd

#endif