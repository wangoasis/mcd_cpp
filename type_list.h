#ifndef _TYPE_LIST_H
#define _TYPE_LIST_H

#include "null_type.h"
#include "type_manip.h"

#define TYPELIST_1(T1) Mcd::Typelist<T1, Mcd::NullType>

#define TYPELIST_2(T1, T2) Mcd::Typelist<T1, TYPELIST_1(T2) >

#define TYPELIST_3(T1, T2, T3) Mcd::Typelist<T1, TYPELIST_2(T2, T3) >

#define TYPELIST_4(T1, T2, T3, T4) \
    Mcd::Typelist<T1, TYPELIST_3(T2, T3, T4) >

#define TYPELIST_5(T1, T2, T3, T4, T5) \
    Mcd::Typelist<T1, TYPELIST_4(T2, T3, T4, T5) >

#define TYPELIST_6(T1, T2, T3, T4, T5, T6) \
    Mcd::Typelist<T1, TYPELIST_5(T2, T3, T4, T5, T6) >

#define TYPELIST_7(T1, T2, T3, T4, T5, T6, T7) \
    Mcd::Typelist<T1, TYPELIST_6(T2, T3, T4, T5, T6, T7) >

#define TYPELIST_8(T1, T2, T3, T4, T5, T6, T7, T8) \
    Mcd::Typelist<T1, TYPELIST_7(T2, T3, T4, T5, T6, T7, T8) >

#define TYPELIST_9(T1, T2, T3, T4, T5, T6, T7, T8, T9) \
    Mcd::Typelist<T1, TYPELIST_8(T2, T3, T4, T5, T6, T7, T8, T9) >

#define TYPELIST_10(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10) \
    Mcd::Typelist<T1, TYPELIST_9(T2, T3, T4, T5, T6, T7, T8, T9, T10) >

namespace Mcd {

    /*
        recursive definition
        for a Typelist containing three types, T should be the first type,
        and U should be a Typelist containing two types.
        By convention, the end of Typelist should be NullType, and it will
        not be counted into the length of Typelist
    */
    template <typename T, typename U>
    struct Typelist {
        typedef T Head;
        typedef U Tail;
    };

    namespace TL {
        /*
            length of Typelist
            computes the length of Typelist at compilation time
        */
        template <typename TList> 
        struct Length;

        template <>
        struct Length<NullType> {
            enum { value = 0 }; 
        };

        template <typename T, typename U>
        struct Length<Typelist<T, U>> {
            enum { value = 1 + Length<U>::value }; 
        };

        /*
            access by index
            if out of bounds, it would show error at compilation time
        */
        template <typename TList, unsigned int i> 
        struct TypeAt;

        template <typename T, typename U>
        struct TypeAt<Typelist<T, U>, 0> {
            typedef T Result;
        };

        template <typename T, typename U, unsigned int i>
        struct TypeAt<Typelist<T, U>, i> {
            typedef typename TypeAt<U, i-1>::Result Result;
        };

        /*
            access by index
            if out of bounds, it would return a default type
        */
        template <typename TList, unsigned int i, typename DefaultType = NullType> 
        struct TypeAtWithDefault {
            typedef DefaultType Result;
        };

        template <typename T, typename U, typename DefaultType>
        struct TypeAtWithDefault<Typelist<T, U>, 0, DefaultType> {
            typedef T Result;
        };

        template <typename T, typename U, unsigned int i, typename DefaultType>
        struct TypeAtWithDefault<Typelist<T, U>, i, DefaultType> {
            typedef typename TypeAtWithDefault<U, i-1, DefaultType>::Result Result;
        };

        /*
            find index of a specific type in Typelist
        */
        template <typename TList, typename T>
        struct IndexOf;

        template <typename T>
        struct IndexOf<NullType, T> {
            enum { value = -1 };
        };

        template <typename T, typename Tail>
        struct IndexOf<Typelist<T, Tail>, T> {
            enum { value = 0 };
        };

        template <typename Head, typename Tail, typename T> // Head is not T
        struct IndexOf<Typelist<Head, Tail>, T> {
        private:
            enum { tmp = IndexOf<Tail, T>::value };
        public:
            enum { value = tmp == -1 ? -1 : 1 + tmp };
        };

        /*
            Append a type or a typelist to an existing Typelist
        */
        template <typename TList, typename T> 
        struct Append;

        template <>
        struct Append<NullType, NullType> {
            typedef NullType Result;
        };

        template <typename T>
        struct Append<NullType, T> {
            typedef TYPELIST_1(T) Result;
        };

        template <typename Head, typename Tail>
        struct Append<NullType, Typelist<Head, Tail>> {
            typedef Typelist<Head, Tail> Result;
        };

        template <typename Head, typename Tail, typename T>
        struct Append<Typelist<Head, Tail>, T> {
            typedef Typelist<Head, typename Append<Tail, T>::Result> Result;
        };

        /*
            Erase the first occurrence of a specific type in a Typelist
        */
        template <typename TList, typename T>
        struct Erase;

        template <typename T>
        struct Erase<NullType, T> {
            typedef NullType Result;
        };

        template <typename T, typename Tail>
        struct Erase<Typelist<T, Tail>, T> {
            typedef Tail Result;
        };

        template <typename Head, typename Tail, typename T>
        struct Erase<Typelist<Head, Tail>, T> {
            typedef Typelist<Head, typename Erase<Tail, T>::Result> Result;
        };

        /*
            Erase all occurrences of a specific type in a Typelist
        */
        template <typename TList, typename T>
        struct EraseAll;

        template <typename T>
        struct EraseAll<NullType, T> {
            typedef NullType Result;
        };

        template <typename T, typename Tail>
        struct EraseAll<Typelist<T, Tail>, T> {
            typedef typename EraseAll<Tail, T>::Result Result;
        };

        template <typename Head, typename Tail, typename T>
        struct EraseAll<Typelist<Head, Tail>, T> {
            typedef Typelist<Head, typename EraseAll<Tail, T>::Result> Result;
        };

        /*
            remove all duplicates
        */
        template <typename TList> 
        struct NoDuplicates;

        template <>
        struct NoDuplicates<NullType> {
            typedef NullType Result;
        };

        template <typename Head, typename Tail>
        struct NoDuplicates<Typelist<Head, Tail>> {
        private:
            typedef typename NoDuplicates<Tail>::Result L1;
            typedef typename Erase<L1, Head>::Result L2;
        public:
            typedef Typelist<Head, L2> Result;
        };

        /*
            Replace the first occurrence of T with U
        */
        template <typename TList, typename T, typename U>
        struct Replace;

        template <typename T, typename U>
        struct Replace<NullType, T, U> {
            typedef NullType Result;
        };

        template <typename T, typename Tail, typename U>
        struct Replace<Typelist<T, Tail>, T, U> {
            typedef Typelist<U, Tail> Result;
        };

        template <typename Head, typename Tail, typename T, typename U>
        struct Replace<Typelist<Head, Tail>, T, U> {
            typedef Typelist<Head, typename Replace<Tail, T, U>::Result> Result;
        };

        /*
            Replace all occurrence of T with U
        */
        template <typename TList, typename T, typename U>
        struct ReplaceAll;

        template <typename T, typename U>
        struct ReplaceAll<NullType, T, U> {
            typedef NullType Result;
        };

        template <typename T, typename Tail, typename U>
        struct ReplaceAll<Typelist<T, Tail>, T, U> {
            typedef Typelist<U, typename ReplaceAll<Tail, T, U>::Result> Result;
        };

        template <typename Head, typename Tail, typename T, typename U>
        struct ReplaceAll<Typelist<Head, Tail>, T, U> {
            typedef Typelist<Head, typename ReplaceAll<Tail, T, U>::Result> Result;
        };

        /*
            reverse a Typelist
        */
        template <typename T>
        struct Reverse;

        template <>
        struct Reverse<NullType> {
            typedef NullType Result;
        };

        template <typename Head, typename Tail>
        struct Reverse<Typelist<Head, Tail>> {
            typedef typename Append<typename Reverse<Tail>::Result, Head>::Result Result;
        };

        /*
            find the most derived class/type from a given base class in a Typelist
        */
        template <typename TList, typename T>
        struct MostDerived;

        template <typename T>
        struct MostDerived<NullType, T> {
            typedef T Result;
        };

        template <typename Head, typename Tail, typename T>
        struct MostDerived<Typelist<Head, Tail>, T> {
        private:
            typedef typename MostDerived<Tail, T>::Result Temp;
        public:
            typedef typename 
                Select<SuperSubclass<Temp, Head>::value, // return 1 if Head is derived from Temp
                Head, Temp>::Result 
                Result;
        };

        /*
            reorder Typelist, make derived classes to front
        */
        template <typename TList>
        struct DerivedToFront;

        template <>
        struct DerivedToFront<NullType> {
            typedef NullType Result; 
        };

        template <typename Head, typename Tail>
        struct DerivedToFront<Typelist<Head, Tail>> {
        private:
            typedef typename MostDerived<Tail, Head>::Result TheMostDerivedFromHead;
            typedef typename Replace<Tail, TheMostDerivedFromHead, Head>::Result Temp;
            typedef typename DerivedToFront<Temp>::Result L;
        public:
            typedef Typelist<TheMostDerivedFromHead, L> Result;
        };
    } // namespace TL
} // namespace Mcd
#endif