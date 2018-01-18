#ifndef _FUNCTOR_H
#define _FUNCTOR_H

#include "small_obj.h"
#include "empty_type.h"
#include "type_list.h"
#include "type_traits.h"
#include <cassert>
#include <typeinfo>
#include <memory>

namespace Mcd {

    namespace Private {
        template <typename R>
        struct FunctorImplBase : public SmallObject<> {

            typedef R ResultType;

            typedef EmptyType Parm1;
            typedef EmptyType Parm2;
            typedef EmptyType Parm3;
            typedef EmptyType Parm4;
            typedef EmptyType Parm5;

            // pure virtual functor
            virtual FunctorImplBase* DoClone() const = 0;

            template <class U>
            static U* Clone(U* pObj) {
                if (!pObj) return 0;
                U* pClone = static_cast<U*>(pObj->DoClone());
                assert(typeid(*pClone) == typeid(*pObj));
                return pClone;
            } 
        };
    } // namespace Private

    /*
        applies Impl design,
        Functor class contains a pointer to FunctorImpl
    */
    template <typename R, typename TList>
    class FunctorImpl;

    // partial template specialization for FunctorImpl of 0-5 parameters
    template <typename R>  // zero parameter
    class FunctorImpl<R, NullType> : public Private::FunctorImplBase<R> {
    public:
        typedef R ResultType; 
        virtual R operator() () = 0; // virtual function takes 0 parameter
    };
    template <typename R, typename P1> // one parameter
    class FunctorImpl<R, TYPELIST_1(P1)> : public Private::FunctorImplBase<R> {
    public:
        typedef R ResultType;
        typedef typename TypeTraits<P1>::ParameterType Parm1;
        virtual R operator() (Parm1) = 0;
    };
    template <typename R, typename P1, typename P2> // two parameter
    class FunctorImpl<R, TYPELIST_2(P1, P2)> : public Private::FunctorImplBase<R> {
    public:
        typedef R ResultType;
        typedef typename TypeTraits<P1>::ParameterType Parm1;
        typedef typename TypeTraits<P2>::ParameterType Parm2;
        virtual R operator() (Parm1, Parm2) = 0;
    };
    template <typename R, typename P1, typename P2, typename P3> // three parameter
    class FunctorImpl<R, TYPELIST_3(P1, P2, P3)> : public Private::FunctorImplBase<R> {
    public:
        typedef R ResultType;
        typedef typename TypeTraits<P1>::ParameterType Parm1;
        typedef typename TypeTraits<P2>::ParameterType Parm2;
        typedef typename TypeTraits<P3>::ParameterType Parm3;
        virtual R operator() (Parm1, Parm2, Parm3) = 0;
    };
    template <typename R, typename P1, typename P2, typename P3, typename P4> // Four parameter
    class FunctorImpl<R, TYPELIST_4(P1, P2, P3, P4)> : public Private::FunctorImplBase<R> {
    public:
        typedef R ResultType;
        typedef typename TypeTraits<P1>::ParameterType Parm1;
        typedef typename TypeTraits<P2>::ParameterType Parm2;
        typedef typename TypeTraits<P3>::ParameterType Parm3;
        typedef typename TypeTraits<P4>::ParameterType Parm4;
        virtual R operator() (Parm1, Parm2, Parm3, Parm4) = 0;
    };
    template <typename R, typename P1, typename P2, typename P3, typename P4, typename P5> // Five parameter
    class FunctorImpl<R, TYPELIST_5(P1, P2, P3, P4, P5)> : public Private::FunctorImplBase<R> {
    public:
        typedef R ResultType;
        typedef typename TypeTraits<P1>::ParameterType Parm1;
        typedef typename TypeTraits<P2>::ParameterType Parm2;
        typedef typename TypeTraits<P3>::ParameterType Parm3;
        typedef typename TypeTraits<P4>::ParameterType Parm4;
        typedef typename TypeTraits<P5>::ParameterType Parm5;
        virtual R operator() (Parm1, Parm2, Parm3, Parm4, Parm5) = 0;
    };


    /*
        FunctorHandler
        for wraping function pointer and Functor together
    */
    template <typename Functor, typename Fp>
    class FunctorHandler : public Functor::Impl {
    private:
        Fp f_; // function pointer 
        typedef typename Functor::Impl Base;
    public:
        typedef typename Base::ResultType ResultType;
        typedef typename Base::Parm1 Parm1;
        typedef typename Base::Parm2 Parm2;
        typedef typename Base::Parm3 Parm3;
        typedef typename Base::Parm4 Parm4;
        typedef typename Base::Parm5 Parm5;

        // constructor from a function pointer
        FunctorHandler(const Fp& fp) : f_(fp)
        {}

        virtual FunctorHandler* DoClone() const { 
            return new FunctorHandler(*this); 
        }

        // function for Functor executing, override the pure virtual function in FunctorImpl
        ResultType operator()() 
        { return f_(); }

        ResultType operator()(Parm1 p1)
        { return f_(p1); }
       
        ResultType operator()(Parm1 p1, Parm2 p2)
        { return f_(p1, p2); }
        
        ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3) 
        { return f_(p1, p2, p3); }
        
        ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4)
        { return f_(p1, p2, p3, p4); }
        
        ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5) 
        { return f_(p1, p2, p3, p4, p5); }
    };

    /*
        MemberFunctorHandler
        for wraping class objects, class member function and Functor together
    */
    template <typename Functor, typename PointerToObj, typename PointerToMemFun>
    class MemberFunctorHandler : public Functor::Impl {
    private:
        PointerToObj pObj_;
        PointerToMemFun pMemFn_;
        typedef typename Functor::Impl Base;
    public:
        typedef typename Base::ResultType ResultType;
        typedef typename Base::Parm1 Parm1;
        typedef typename Base::Parm2 Parm2;
        typedef typename Base::Parm3 Parm3;
        typedef typename Base::Parm4 Parm4;
        typedef typename Base::Parm5 Parm5;

        // constructor
        MemberFunctorHandler(const PointerToObj& pObj, PointerToMemFun pMemFn) 
        : pObj_(pObj), pMemFn_(pMemFn)
        {}

        virtual MemberFunctorHandler* DoClone() const { 
            return new MemberFunctorHandler(*this); 
        }

        // function for Functor executing, override the pure virtual function in FunctorImpl
        // calling member function of a class
        ResultType operator()() 
        { return (*pObj_.*pMemFn_)(); }

        ResultType operator()(Parm1 p1)
        { return (*pObj_.*pMemFn_)(p1); }
       
        ResultType operator()(Parm1 p1, Parm2 p2)
        { return (*pObj_.*pMemFn_)(p1, p2); }
        
        ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3) 
        { return (*pObj_.*pMemFn_)(p1, p2, p3); }
        
        ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4)
        { return (*pObj_.*pMemFn_)(p1, p2, p3, p4); }
        
        ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5) 
        { return (*pObj_.*pMemFn_)(p1, p2, p3, p4, p5); }
    };

    /*
        Functor
    */
    template <typename R, typename TList = NullType>
    class Functor {
    public:
        // Functor class will contains a pointer to Impl
        typedef FunctorImpl<R, TList> Impl;
    private:
        std::unique_ptr<Impl> spImpl_;
    public:
        typedef R ResultType;
        typedef TList ParamList;   
        typedef typename Impl::Parm1 Parm1;
        typedef typename Impl::Parm2 Parm2;
        typedef typename Impl::Parm3 Parm3;
        typedef typename Impl::Parm4 Parm4;
        typedef typename Impl::Parm5 Parm5;

        // constructor
        Functor() : spImpl_() 
        {}

        //copy constructor
        Functor(const Functor& rhs)
        // unique_ptr.get() returns the raw pointer
        : spImpl_(Impl::Clone(rhs.spImpl_.get()))
        {}

        Functor(std::unique_ptr<Impl> spImpl) 
        // here using std::move is very crucial, because unique_ptr has delete the copy
        // constructor copying from another unique_ptr object
        : spImpl_(std::move(spImpl))
        {}

        /* 
            constuctor from a function pointer
            FunctorHandler is defined above, it's a helper class for wraping function pointer with Functor
            Since spImpl_ are given value as a new FunctorHandler object, therefore,
            it's obvious that FunctorHandler should derive from FunctorImpl
        */
        template <typename Fun>
        Functor(Fun fun)
        : spImpl_(new FunctorHandler<Functor, Fun>(fun))
        {}

        /* 
            constuctor from a class and its member function
            MemberFunctorHandler is defined above, it's a helper class for wraping class object, it
            member function with Functor together.
        */
        template <class PtrObj, typename MemFn>
        Functor(const PtrObj& p, MemFn memFn)
        : spImpl_(new MemberFunctorHandler<Functor, PtrObj, MemFn>(p, memFn))
        {}

        // function for Functor executing, override the pure virtual function in FunctorImpl
        ResultType operator()() const
        { return (*spImpl_)(); }

        ResultType operator()(Parm1 p1) const
        { return (*spImpl_)(p1); }
        
        ResultType operator()(Parm1 p1, Parm2 p2) const
        { return (*spImpl_)(p1, p2); }
        
        ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3) const
        { return (*spImpl_)(p1, p2, p3); }
        
        ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4) const
        { return (*spImpl_)(p1, p2, p3, p4); }
        
        ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5) const
        { return (*spImpl_)(p1, p2, p3, p4, p5); }
    };

    namespace Private
    {
        template <class Fctor> struct BinderFirstTraits;

        template <typename R, class TList>
        struct BinderFirstTraits< Functor<R, TList> >
        {
            typedef typename TL::Erase<TList, 
                    typename TL::TypeAt<TList, 0>::Result>::Result
                ParmList;
            typedef Functor<R, ParmList> BoundFunctorType;
            typedef typename BoundFunctorType::Impl Impl;
        };        
    } // namespace Private

    /*
        Binder template
    */ 
    template <class OriginalFunctor>
    class BinderFirst : public Private::BinderFirstTraits<OriginalFunctor>::Impl {

        typedef typename Private::BinderFirstTraits<OriginalFunctor>::Impl Base;
        typedef typename OriginalFunctor::ResultType ResultType;

        typedef typename OriginalFunctor::Parm1 BoundType;

        typedef typename OriginalFunctor::Parm2 Parm1;
        typedef typename OriginalFunctor::Parm3 Parm2;
        typedef typename OriginalFunctor::Parm4 Parm3;
        typedef typename OriginalFunctor::Parm5 Parm4;
        typedef EmptyType Parm5;

    public:
        BinderFirst(const OriginalFunctor& fun, BoundType bound)
        : f_(fun), b_(bound)
        {}
        
        virtual BinderFirst* DoClone() const { 
            return new BinderFirst(*this); 
        }
        
        // operator() implementations for up to 5 arguments
                
        ResultType operator()()
        { return f_(b_); }

        ResultType operator()(Parm1 p1)
        { return f_(b_, p1); }
        
        ResultType operator()(Parm1 p1, Parm2 p2)
        { return f_(b_, p1, p2); }
        
        ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3)
        { return f_(b_, p1, p2, p3); }
        
        ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4)
        { return f_(b_, p1, p2, p3, p4); }
        
    private:
        OriginalFunctor f_;
        BoundType b_;
    };
    
    template <class Fctor>
    typename Private::BinderFirstTraits<Fctor>::BoundFunctorType
    BindFirst(
        const Fctor& fun,
        typename Fctor::Parm1 bound)
    {
        typedef typename Private::BinderFirstTraits<Fctor>::BoundFunctorType
            Outgoing;
        
        return Outgoing(std::unique_ptr<typename Outgoing::Impl>(
            new BinderFirst<Fctor>(fun, bound)));
    }
} // namespace Mcd

#endif