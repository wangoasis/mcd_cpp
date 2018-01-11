#ifndef _HIERARCHY_GENERATORS_H
#define _HIERARCHY_GENERATORS_H

#include "type_list.h"
#include "type_manip.h"
#include "type_traits.h"

namespace Mcd {

    namespace Tag {
        template<class, class> 
        struct ScatterHierarchyTag;
    } // namespace Tag
    /*
        GenScatterHierarchy generates a hierarchy that instantiates Unit 
        with each type in the TList. A instantiation of GenScatterHierarchy
        direcyly or indirectly inherits Unit<T> for each T in TList

        For example:
        Unit can be:
            template <class T>
            struct Holder {
                T value_;
            }

            typedef GenScatterHierarchy<TYPELIST_3(WidgetName, WidgetSize, WidgetProperty), Holder> WidgetInfo;
    */
    template <typename TList, template <typename> class Unit>
    class GenScatterHierarchy; 

    /*
        specilization from Typelist
    */
    template <typename Head, typename Tail, template <typename> class Unit>
    class GenScatterHierarchy<Typelist<Head, Tail>, Unit> 
    :   public GenScatterHierarchy<Tag::ScatterHierarchyTag<Head, Tail>, Unit>, 
        public GenScatterHierarchy<Tail, Unit> // inherit from Head and Tail
    {
    public:
        typedef GenScatterHierarchy<Tag::ScatterHierarchyTag<Head, Tail>, Unit> LeftBase; 
        typedef GenScatterHierarchy<Tail, Unit> RightBase;
        // for retrieving Unit<T> with T and given Typelist
        template <typename T> 
        struct Rebind {
            typedef Unit<T> Result;
        };

        typedef Typelist<Head, Tail> TList; // used in FieldHelper function
    };

    /* 
        refer to line 40 
        typedef GenScatterHierarchy<Tag::ScatterHierarchyTag<Head, Tail>, Unit> LeftBase;
        we need to extract T1, and we don't need T2 here
    */
    template <class T1, class T2, template <class> class Unit>
    class GenScatterHierarchy<Tag::ScatterHierarchyTag<T1, T2>, Unit> 
    :   public GenScatterHierarchy<T1, Unit>
    {
    };

    /*
        specialization from a specific type
    */
    template <typename AtomicType, template <typename> class Unit>
    class GenScatterHierarchy
    :   public Unit<AtomicType> 
    {
        typedef Unit<AtomicType> LeftBase;

    public:
        // for retrieving Unit<T> with T and given Typelist
        template <typename T> 
        struct Rebind {
            typedef Unit<T> Result;
        };
    };

    /*
        specialization from NullType
    */
    template <template <typename> class Unit>
    class GenScatterHierarchy<NullType, Unit> 
    {
    public:
        // for retrieving Unit<T> with T and given Typelist
        template <typename T> 
        struct Rebind {
            typedef Unit<T> Result;
        };
    };

    /*
        Field<type>(GenScatterHierarchy obj)
        returns a reference to the Unit<type> object in GenScatterHierarchy obj
    */
    template <typename T, typename GenScatterHierarchyObject>
    typename GenScatterHierarchyObject::template Rebind<T>::Result& 
    // return value type: Unit<T>, since Unit is not known from GenScatterHierarchyObject, thus need Rebind 
    // method defined in GenScatterHierarchy template
    Field(GenScatterHierarchyObject& obj) {
        /*
            The reference of GenScatterHierarchyObject can be binded to a Unit<T> object,
            since obj is the most derived one, it derives from every Unit<type>
        */
        return obj;
    }
    // const version
    template <typename T, typename GenScatterHierarchyObject>
    const typename GenScatterHierarchyObject::template Rebind<T>::Result& 
    Field(const GenScatterHierarchyObject& obj) {
        return obj;
    }

    /*
        definitions of Tuple
        example of Tuple:
            typedef Tuple<TYPELIST_3(int, int, int)> Point3D;
            Point3D point;
            Field<0>(point) = 3;
            Field<1>(point) = 4;
            Field<2>(point) = 2;
    */
    // Holder of Tuple element
    template <typename T>
    struct TupleUnit {
        T value_;
        // overload of () operator, no longer need ".value_" to access the value_
        operator T& () {
            return value_;
        } 
        operator const T& () const {
            return value_;
        }
    };

    template <typename TList>
    struct Tuple : public GenScatterHierarchy<TList, TupleUnit> {

    };

    /*
        helper function for Field
    */
    template <typename H, int i>
    struct FieldHelper;

    // specialiazion when index equal 0
    template <class H>
    struct FieldHelper<H, 0>
    {
        typedef typename H::TList::Head ElementType; // index 0
        typedef typename H::template Rebind<ElementType>::Result UnitType; // Unit<ElementType>
        
        enum {
            isTuple = Conversion<UnitType, TupleUnit<ElementType> >::sameType,
            isConst = TypeTraits<H>::isConst
        };

        typedef const typename H::LeftBase ConstLeftBase;
        
        typedef typename Select<isConst, ConstLeftBase, 
            typename H::LeftBase>::Result LeftBase;
            
        typedef typename Select<isTuple, ElementType, 
            UnitType>::Result UnqualifiedResultType;

        typedef typename Select<isConst, const UnqualifiedResultType,
                        UnqualifiedResultType>::Result ResultType;
            
        static ResultType& Do(H& obj)
        {
            LeftBase& leftBase = obj;
            return leftBase;
        }
    };

    template <class H, int i>
    struct FieldHelper
    {
        typedef typename TL::TypeAt<typename H::TList, i>::Result ElementType;
        typedef typename H::template Rebind<ElementType>::Result UnitType;
        
        enum {
            isTuple = Conversion<UnitType, TupleUnit<ElementType> >::sameType,
            isConst = TypeTraits<H>::isConst
        };

        typedef const typename H::RightBase ConstRightBase;
        
        typedef typename Select<isConst, ConstRightBase, 
            typename H::RightBase>::Result RightBase;

        typedef typename Select<isTuple, ElementType, 
            UnitType>::Result UnqualifiedResultType;

        typedef typename Select<isConst, const UnqualifiedResultType,
                        UnqualifiedResultType>::Result ResultType;
            
        static ResultType& Do(H& obj) {
            RightBase& rightBase = obj;
            return FieldHelper<RightBase, i - 1>::Do(rightBase);
        }
    };

    /*
        Field<int>(GenScatterHierarchy obj)
        returns a reference to the proper type of i-th Unit<type> in a GenScatterHierarchy obj
    */
    template <int i, typename H>
    typename FieldHelper<H, i>::ResultType& // return type
    Field(H& obj) {
        return FieldHelper<H, i>::Do(obj);
    }
} // namespace Mcd

#endif