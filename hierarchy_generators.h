#ifndef _HIERARCHY_GENERATORS_H
#define _HIERARCHY_GENERATORS_H

#include "type_list.h"

namespace Mcd {

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
    :   public GenScatterHierarchy<Head, Unit>, 
        public GenScatterHierarchy<Tail, Unit> // inherit from Head and Tail
    {
        typedef GenScatterHierarchy<Head, Unit> LeftBase; 
        typedef GenScatterHierarchy<Tail, Unit> RightBase;
    public:
        // for retrieving Unit<T> with T and given Typelist
        template <typename T> 
        struct Rebind {
            typedef Unit<T> Result;
        };
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
            obj can be casted into Unit<T>, since obj is the most derived one,
            it derives from every Unit<type>
        */
        return obj;
    }
    // const version
    template <typename T, typename GenScatterHierarchyObject>
    const typename GenScatterHierarchyObject::template Rebind<T>::Result& 
    Field(const GenScatterHierarchyObject& obj) {
        return obj;
    }
} // namespace Mcd

#endif