#ifndef _TYPE_MANIP_H
#define _TYPE_MANIP_H

namespace Mcd {

    template <int v>
    struct Int2Type {
        enum {value = v};
    };

    template <typename T>
    struct Type2Type {
        typedef T OriginalType;
    };

    /*
        select type T or U based on boolean flag's value
    */
    template <bool flag, typename T, typename U>
    struct Select {
        typedef T Result;
    };
    template <typename T, typename U>
    struct Select<false, T, U> {
        typedef U Result;
    };

    template <typename T, typename U>
    struct IsSameType {
        enum {value = false};
    };
    template <typename T>
    struct IsSameType<T, T> {
        enum {value = true};
    };

    namespace Private {
        template <typename T, typename U>
        struct ConversionHelper {
            typedef char Small;
            struct Big { 
                char dummy[2];
            };
            static Small Test(U);
            static Big Test(...);
            static T makeT();
        };
    }

    /*
        struct to check whether T can convert to U
    */
    template <class T, class U>
    struct Conversion
    {
        typedef Private::ConversionHelper<T, U> H;

        /*
            if T can be converted to U, then it would call Test(U) function and returns a Small object,
            exists woul be true;
            if T can not be converted to U, then it would call Test(...) function and returns a Big object,
            exists would be false;
        */
        enum { exists = sizeof(typename H::Small) == sizeof((H::Test(H::makeT()))) };

        // T can be converted to U, and U can be converted to T
        enum { exists2Way = exists && Conversion<U, T>::exists };

        enum { sameType = false };
    };
    
    // partial template specialization
    template <class T>
    struct Conversion<T, T>    
    {
        enum { exists = 1, exists2Way = 1, sameType = 1 };
    };
    
    template <class T>
    struct Conversion<void, T>    
    {
        enum { exists = 0, exists2Way = 0, sameType = 0 };
    };
    
    template <class T>
    struct Conversion<T, void>    
    {
        enum { exists = 0, exists2Way = 0, sameType = 0 };
    };
    
    template <>
    struct Conversion<void, void>    
    {
    public:
        enum { exists = 1, exists2Way = 1, sameType = 1 };
    };

    /*
        struct to check whether T is a public base class of U, or T and U is the sameType
    */
    template <class T, class U>
    struct SuperSubclass
    {
      enum { value = (Conversion<const U*, const T*>::exists && // U can be converted to T
                      !Conversion<const T*, const void*>::sameType) // T is not void
             };
    };

    // Returns true if T is a public base of U
    template<class T,class U>
    struct SuperSubclassStrict
    {
      enum { value = (Conversion<const U*, const T*>::exists && // U can be converted to T
                     !Conversion<const T*, const void*>::sameType && // T is not void
                     !Conversion<const T*, const U*>::sameType) // T and U is not same type
            };
    };
} // namespace Mcd

/*
    check whether T is a public base class of U, or T and U is the sameType
*/
#define SUPER_SUBCLASS(T, U) Mcd::SuperSubclass<T,U>::value

/*
    check whether T is a public base class of U
*/
#define SUPER_SUBCLASS_STRICT(T, U) Mcd::SuperSubclassStrict<T,U>::value

#define IS_SAME_TYPE(T, U) Mcd::IsSameType<T,U>::value

#endif