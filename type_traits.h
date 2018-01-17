#ifndef _TYPE_TRAITS_H
#define _TYPE_TRAITS_H

#include "null_type.h"

namespace Mcd {
    namespace Private {
        typedef TYPELIST_4(unsigned char, unsigned short int,
           unsigned int, unsigned long int) StdUnsignedInts;
        typedef TYPELIST_4(signed char, short int,
           int, long int) StdSignedInts;
        typedef TYPELIST_3(bool, char, wchar_t) StdOtherInts;
        typedef TYPELIST_3(float, double, long double) StdFloats;

        template <class U> struct AddReference
        {
            typedef U & Result;
        };

        template <class U> struct AddReference<U &>
        {
            typedef U & Result;
        };

        template <> struct AddReference<void>
        {
            typedef NullType Result;
        };
    } // namespace Private

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

        /*
            check member pointer
        */
        template <class U> struct PToMTraits {
            enum { result = false };
        };
        
        template <class U, class V>
        struct PToMTraits<U V::*> {
            enum { result = true };
        };;

    public:
        enum { isPointer = PointerTraits<T>::result };
        typedef typename PointerTraits<T>::PointeeType PointeeType;

        enum { isReference = ReferenceTraits<T>::result };
        typedef typename ReferenceTraits<T>::ReferredType ReferredType;

        enum { isConst = ConstTraits<T>::result };
        typedef typename ConstTraits<T>::ResultType NonConstType;

        enum { isMemberPointer = PToMTraits<T>::result };
    
        enum { isStdUnsignedInt = 
            TL::IndexOf<Private::StdUnsignedInts, T>::value >= 0 };
        enum { isStdSignedInt = 
            TL::IndexOf<Private::StdSignedInts, T>::value >= 0 };
        enum { isStdIntegral = isStdUnsignedInt || isStdSignedInt ||
            TL::IndexOf<Private::StdOtherInts, T>::value >= 0 };
        enum { isStdFloat = TL::IndexOf<Private::StdFloats, T>::value >= 0 }; 
        enum { isStdArith = isStdIntegral || isStdFloat };
        typedef typename Select<isStdArith || isPointer || isMemberPointer,
            T, typename Private::AddReference<T>::Result>::Result ParameterType;
    };
} // namespace Mcd

#endif