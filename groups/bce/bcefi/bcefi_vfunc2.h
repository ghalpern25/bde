// bcefi_vfunc2.h               -*-C++-*-
#ifndef INCLUDED_BCEFI_VFUNC2
#define INCLUDED_BCEFI_VFUNC2

#ifndef INCLUDED_BDES_IDENT
#include <bdes_ident.h>
#endif
BDES_IDENT("$Id: $")


//@PURPOSE: Provide concrete representations for 2-arg void function objects.
//
//@CLASSES:
// bcefi_Vfunc2Null: a 2-arg void no-op functor
//
// bcefi_Vfunc2F2: a 2-arg void functor created from a 2-arg function
// bcefi_Vfunc2F3: a 2-arg void functor created from a 3-arg function
// bcefi_Vfunc2F4: a 2-arg void functor created from a 4-arg function
// bcefi_Vfunc2F5: a 2-arg void functor created from a 5-arg function
// bcefi_Vfunc2F6: a 2-arg void functor created from a 6-arg function
// bcefi_Vfunc2F7: a 2-arg void functor created from a 7-arg function
// bcefi_Vfunc2F8: a 2-arg void functor created from a 8-arg function
// bcefi_Vfunc2F9: a 2-arg void functor created from a 9-arg function
//
// bcefi_Vfunc2C2: a 2-arg void functor created from obj & 2-arg method
// bcefi_Vfunc2C3: a 2-arg void functor created from obj & 3-arg method
// bcefi_Vfunc2C4: a 2-arg void functor created from obj & 4-arg method
// bcefi_Vfunc2C5: a 2-arg void functor created from obj & 5-arg method
// bcefi_Vfunc2C6: a 2-arg void functor created from obj & 6-arg method
// bcefi_Vfunc2C7: a 2-arg void functor created from obj & 7-arg method
// bcefi_Vfunc2C8: a 2-arg void functor created from obj & 8-arg method
// bcefi_Vfunc2C9: a 2-arg void functor created from obj & 9-arg method
//
// bcefi_Vfunc2M2: a 2-arg void functor created from obj ptr & 2-arg method
// bcefi_Vfunc2M3: a 2-arg void functor created from obj ptr & 3-arg method
// bcefi_Vfunc2M4: a 2-arg void functor created from obj ptr & 4-arg method
// bcefi_Vfunc2M5: a 2-arg void functor created from obj ptr & 5-arg method
// bcefi_Vfunc2M6: a 2-arg void functor created from obj ptr & 6-arg method
// bcefi_Vfunc2M7: a 2-arg void functor created from obj ptr & 7-arg method
// bcefi_Vfunc2M8: a 2-arg void functor created from obj ptr & 8-arg method
// bcefi_Vfunc2M9: a 2-arg void functor created from obj ptr & 9-arg method
//
//@AUTHOR: John Lakos (jlakos)
//
//@DEPRECATED: This component should not be used in new code, and will be
// deleted in the near future.  Please see 'bdef_function', 'bdef_bind', etc.
// for alternatives that should be used for all future development.
//
//@DESCRIPTION: This component contains a suite of classes to implement the
// polymorphic representation of a function object (functor) whose
// function-call operator ('execute') takes two arguments and returns 'void'
// (hence the root name 'vfunc2' -- "v" for 'void' and "2" for the number of
// arguments).  Direct clients of this component choose a concrete
// implementation of the base 'bcefr_Vfunc2' representation based on the
// number of additional arguments required by the underlying procedure, and on
// the specific kind or "flavor" of function that the functor is to invoke.
// If the user does not wish to invoke any function on the functor invocation,
// then the "null functor" 'bcefi_Vfunc2Null' may be used.
//
///Language-Imposed Details
///------------------------
// For practical reasons, this component must distinguish among four types of
// functors.  The first type is a "null functor" 'bcefi_Vfunc2Null', which
// differs from the other three types in that it does not invoke any function.
//
// The other functors are distinguished by three flavors 'X' of functions they
// invoke, and also by the total number 'N' of arguments (N >= 2) that the
// function takes.  This component supplies a separate concrete implementation
// for each of the specified types.  These 24 non-null concrete
// implementations are named 'bcefi_Vfunc2XN' where 'X' is one of {'F', 'C',
// 'M'} and 'N' is in the range [2..9].
//
// The possible function flavors (and labels) are classified as:
//..
//     Label   Classification
//     -----   --------------
//       F     free function, static member function, or "free" functor object
//       C     'const' (accessor) member function
//       M     non-'const' (manipulator) member function
//..
// Note that a 'const' member function may be wrapped by a 'bcefi_Vfunc2MN'
// functor, but wrapping a non-'const' method by a 'bcefi_Vfunc2CN' will
// result in compile time error in most compiler implementations.
//
// Note also that, to accommodate the preservation of 'const', the
// 'bcefi_Vfunc2CN' functors hold an object 'Obj', supplied by a 'const'
// 'Obj&' in the constructor, while 'bcefi_Vfunc2MN' functors hold a pointer
// to 'Obj' supplied by a modifiable 'Obj*' argument in the constructor.
//
// The (N - 2) additional arguments specified at the construction of the
// functor are held by the functor, and appended, in order, to the two
// required (invocation-specific) arguments specified in each call to the
// virtual 'execute' method when the underlying function is invoked by
// 'execute'.  Any value returned from the underlying procedure is ignored.
//
// Note that the respective types of the parameters of the underlying procedure
// must correspond to the overall parameterization of the particular template
// representation class.  The virtual 'execute' method in each representation
// class defined in this component takes two arguments.  The respective types
// of the two leading parameters of the underlying procedure must match (or be
// automatically convertible from) those of the parameterized template
// representation class's virtual 'execute' method.  The respective types of
// any additional trailing parameters to the underlying procedure must match
// (or be convertible from) those specified in the particular representation
// class's constructor.
//
///USAGE
///-----
// Given the following free function, which takes four arguments (and happens
// to return an 'int' value)
//..
//   int myFunction(int a, double b, const char *c, int d);
//..
// we can create a (dynamically-allocated) two-argument functor object that, at
// construction, wraps up the function along with the two additional
// arguments, and subsequently refer to this 'bcefi_Vfunc2F4' functor only
// through its base class, 'bcefr_Vfunc2':
//..
//   typedef int (*MyFunctionPtr)(int, double, const char *, int);
//   typedef bcefi_Vfunc2F4<MyFunctionPtr,
//                        int, double, const char *, int> MyFunctorRep;
//
//   bslma_Allocator *ba = bslma_Default::allocator(0);
//
//   bcefr_Vfunc2<int, double> *rep =
//                           new(ba) MyFunctorRep(myFunction, "3", 4, ba);
//..
// The virtual 'execute' method (taking two arguments) can now be called
// through the 'bcefr_Vfunc2' base class pointer 'rep'.  This method will, in
// turn, invoke the underlying function with two arguments supplied by the
// caller of the 'execute' method, concatenated with the sequence of 2
// additional (trailing) arguments supplied at construction.
//..
//   rep->execute(10, 20.75);
//..
// The derived object is destroyed through the 'deleteObject' class method of
// the base class, which allows us to declare its destructor 'private'.
//..
//   bcefr_Vfunc2<int, double>::deleteObject(rep);
//..
// This mechanism allows us to hide the details of destruction and memory
// deallocation from the user of this class.  By declaring the destructor
// private we also prevent creation of (automatic) instances on the program
// stack, which precludes accidental use of 'delete' and also prevents further
// derivation.
//
// Finally, note that the integer returned to the functor by 'myFunction' is
// ignored.

#ifndef INCLUDED_BCESCM_VERSION
#include <bcescm_version.h>
#endif

#ifndef INCLUDED_BSLALG_CONSTRUCTORPROXY
#include <bslalg_constructorproxy.h>
#endif

#ifndef INCLUDED_BCEFR_VFUNC2
#include <bcefr_vfunc2.h>
#endif


namespace BloombergLP {

                        // ======================
                        // class bcefi_Vfunc2Null
                        // ======================

template <class A1, class A2>
class bcefi_Vfunc2Null : public bcefr_Vfunc2<A1, A2> {
    // This class defines the representation for a function object (functor),
    // characterized by a function-call operator taking two arguments,
    // returning 'void', and performing no operations.

    // NOT IMPLEMENTED
    bcefi_Vfunc2Null(const bcefi_Vfunc2Null<A1, A2>&);
    bcefi_Vfunc2Null<A1, A2>& operator=(const bcefi_Vfunc2Null<A1, A2>&);

  private:
    // PRIVATE CREATORS
    ~bcefi_Vfunc2Null();
        // Destroy this functor.  Note that this destructor can be invoked only
        // indirectly, via the static 'deleteObject' method of the base class.

    // PRIVATE ACCESSORS
    void execute(const A1& argument1, const A2& argument2) const;
        // Invoke the no-op function with the specified 'argument1' and
        // 'argument2'.

  public:
    // CREATORS
    bcefi_Vfunc2Null(bslma_Allocator *basicAllocator);
        // Create a representation for a function object (functor) taking two
        // arguments, returning 'void', and performing no operation.
};

                        // ====================
                        // class bcefi_Vfunc2F2
                        // ====================

template <class F, class A1, class A2>
class bcefi_Vfunc2F2 : public bcefr_Vfunc2<A1, A2> {
    // This class defines the representation for a function object (functor),
    // characterized by a function-call operator taking two arguments and
    // returning 'void', that holds a pure procedure (i.e., free function,
    // static member function, or functor) taking no additional arguments.

    // DATA
    bslalg_ConstructorProxy<F> d_f;  // function pointer (or functor)

    // NOT IMPLEMENTED
    bcefi_Vfunc2F2(const bcefi_Vfunc2F2<F, A1, A2>&);
    bcefi_Vfunc2F2<F, A1, A2>& operator=(const bcefi_Vfunc2F2<F, A1, A2>&);

  private:
    // PRIVATE CREATORS
    ~bcefi_Vfunc2F2();
        // Destroy this functor.  Note that this destructor can be invoked only
        // indirectly, via the static 'deleteObject' method of the base class.

    // PRIVATE ACCESSORS
    void execute(const A1& argument1, const A2& argument2) const;
        // Invoke the underlying procedure (free function, static member
        // function, or functor) with the specified 'argument1' and
        // 'argument2' (and no additional arguments).

  public:
    // CREATORS
    bcefi_Vfunc2F2(F procedure, bslma_Allocator *basicAllocator);
        // Create a representation for a function object (functor) taking two
        // arguments and returning 'void', using the specified 'procedure'
        // (i.e., free function, static member function, or functor) taking no
        // additional arguments.  Use the specified 'basicAllocator' to supply
        // memory.  Note that any value returned from 'procedure' will be
        // ignored.
};

                        // ====================
                        // class bcefi_Vfunc2F3
                        // ====================

template <class F, class A1, class A2, class D1>
class bcefi_Vfunc2F3 : public bcefr_Vfunc2<A1, A2> {
    // This class defines the representation for a function object (functor),
    // characterized by a function-call operator taking two arguments and
    // returning 'void', that holds a pure procedure (i.e., free function,
    // static member function, or functor) taking one additional trailing
    // argument, and that argument's corresponding value.

    // DATA
    bslalg_ConstructorProxy<F>  d_f;   // function pointer (or functor)
    bslalg_ConstructorProxy<D1> d_d1;  // first embedded argument

    // NOT IMPLEMENTED
    bcefi_Vfunc2F3(const bcefi_Vfunc2F3<F, A1, A2, D1>&);
    bcefi_Vfunc2F3<F, A1, A2, D1>& operator=(
                   const bcefi_Vfunc2F3<F, A1, A2, D1>&);

  private:
    // PRIVATE CREATORS
    ~bcefi_Vfunc2F3();
        // Destroy this functor.  Note that this destructor can be only
        // indirectly, via the static 'deleteObject' method of the base class.

    // PRIVATE ACCESSORS
    void execute(const A1& argument1, const A2& argument2) const;
        // Invoke the underlying procedure (free function, static member
        // function, or functor) with the specified 'argument1' and
        // 'argument2' along with the trailing argument value specified at
        // construction.

  public:
    // CREATORS
    bcefi_Vfunc2F3(F                procedure,
                   const D1&        embeddedArg1,
                   bslma_Allocator *basicAllocator);
        // Create a representation for a function object (functor) taking two
        // arguments and returning 'void', using the specified 'procedure'
        // (i.e., free function, static member function, or functor) taking
        // one additional trailing argument, and this corresponding argument's
        // specified 'embeddedArg1' value.  Use the specified 'basicAllocator'
        // to supply memory.  Note that any value returned from 'procedure'
        // will be ignored.
};

                        // ====================
                        // class bcefi_Vfunc2F4
                        // ====================

template <class F, class A1, class A2, class D1, class D2>
class bcefi_Vfunc2F4 : public bcefr_Vfunc2<A1, A2> {
    // This class defines the representation for a function object (functor),
    // characterized by a function-call operator taking two arguments and
    // returning 'void', that holds a pure procedure (i.e., free function,
    // static member function, or functor) taking two additional trailing
    // arguments, and those arguments' corresponding values.

    // DATA
    bslalg_ConstructorProxy<F>  d_f;   // function pointer (functor)
    bslalg_ConstructorProxy<D1> d_d1;  // first embedded argument
    bslalg_ConstructorProxy<D2> d_d2;  // second embedded argument

    // NOT IMPLEMENTED
    bcefi_Vfunc2F4(const bcefi_Vfunc2F4<F, A1, A2, D1, D2>&);
    bcefi_Vfunc2F4<F, A1, A2, D1, D2>& operator=(
                   const bcefi_Vfunc2F4<F, A1, A2, D1, D2>&);

  private:
    // PRIVATE CREATORS
    ~bcefi_Vfunc2F4();
        // Destroy this functor.  Note that this destructor can be invoked only
        // indirectly, via the static 'deleteObject' method of the base class.

    // PRIVATE ACCESSORS
    void execute(const A1& argument1, const A2& argument2) const;
        // Invoke the underlying procedure (free function, static member
        // function, or functor) with the specified 'argument1' and
        // 'argument2' along with the sequence of two argument values
        // specified at construction.

  public:
    // CREATORS
    bcefi_Vfunc2F4(F                procedure,
                   const D1&        embeddedArg1,
                   const D2&        embeddedArg2,
                   bslma_Allocator *basicAllocator);
        // Create a representation for a function object (functor) taking two
        // arguments and returning 'void', using the specified 'procedure'
        // (i.e., free function, static member function, or functor) taking
        // two additional trailing arguments, and the corresponding arguments'
        // specified 'embeddedArg1' and 'embeddedArg2' values.  Use the
        // specified 'basicAllocator' to supply memory.  Note that any value
        // returned from 'procedure' will be ignored.
};

                        // ====================
                        // class bcefi_Vfunc2F5
                        // ====================

template <class F, class A1, class A2, class D1, class D2, class D3>
class bcefi_Vfunc2F5 : public bcefr_Vfunc2<A1, A2> {
    // This class defines the representation for a function object (functor),
    // characterized by a function-call operator taking two arguments and
    // returning 'void', that holds a pure procedure (i.e., free function,
    // static member function, or functor) taking three additional trailing
    // arguments, and those arguments' corresponding values.

    // DATA
    bslalg_ConstructorProxy<F>  d_f;   // function pointer (or functor)
    bslalg_ConstructorProxy<D1> d_d1;  // first embedded argument
    bslalg_ConstructorProxy<D2> d_d2;  // second embedded argument
    bslalg_ConstructorProxy<D3> d_d3;  // third embedded argument

    // NOT IMPLEMENTED
    bcefi_Vfunc2F5(const bcefi_Vfunc2F5<F, A1, A2, D1, D2, D3>&);
    bcefi_Vfunc2F5<F, A1, A2, D1, D2, D3>& operator=(
                   const bcefi_Vfunc2F5<F, A1, A2, D1, D2, D3>&);

  private:
    // PRIVATE CREATORS
    ~bcefi_Vfunc2F5();
        // Destroy this functor.  Note that this destructor can be invoked only
        // indirectly, via the static 'deleteObject' method of the base class.

    // PRIVATE ACCESSORS
    void execute(const A1& argument1, const A2& argument2) const;
        // Invoke the underlying procedure (free function, static member
        // function, or functor) with the specified 'argument1' and
        // 'argument2' along with the sequence of three argument values
        // specified at construction.

  public:
    // CREATORS
    bcefi_Vfunc2F5(F                procedure,
                   const D1&        embeddedArg1,
                   const D2&        embeddedArg2,
                   const D3&        embeddedArg3,
                   bslma_Allocator *basicAllocator);
        // Create a representation for a function object (functor) taking two
        // arguments and returning 'void', using the specified 'procedure'
        // (i.e., free function, static member function, or functor) taking
        // three additional trailing arguments and the corresponding
        // arguments' specified 'embeddedArg1', 'embeddedArg2, and
        // 'embeddedArg3' values.  Use the specified 'basicAllocator' to
        // supply memory.  Note that any value returned from 'procedure' will
        // be ignored.
};

                        // ====================
                        // class bcefi_Vfunc2F6
                        // ====================

template <class F, class A1, class A2, class D1, class D2, class D3, class D4>
class bcefi_Vfunc2F6 : public bcefr_Vfunc2<A1, A2> {
    // This class defines the representation for a function object (functor),
    // characterized by a function-call operator taking two arguments and
    // returning 'void', that holds a pure procedure (i.e., free function,
    // static member function, or functor) taking four additional trailing
    // arguments, and those arguments' corresponding values.

    // DATA
    bslalg_ConstructorProxy<F>  d_f;   // function pointer (or functor)
    bslalg_ConstructorProxy<D1> d_d1;  // first embedded argument
    bslalg_ConstructorProxy<D2> d_d2;  // second embedded argument
    bslalg_ConstructorProxy<D3> d_d3;  // third embedded argument
    bslalg_ConstructorProxy<D4> d_d4;  // fourth embedded argument

    // NOT IMPLEMENTED
    bcefi_Vfunc2F6(const bcefi_Vfunc2F6<F, A1, A2, D1, D2, D3, D4>&);
    bcefi_Vfunc2F6<F, A1, A2, D1, D2, D3, D4>& operator=(
                   const bcefi_Vfunc2F6<F, A1, A2, D1, D2, D3, D4>&);

  private:
    // PRIVATE CREATORS
    ~bcefi_Vfunc2F6();
        // Destroy this functor.  Note that this destructor can be invoked only
        // indirectly, via the static 'deleteObject' method of the base class.

    // PRIVATE ACCESSORS
    void execute(const A1& argument1, const A2& argument2) const;
        // Invoke the underlying procedure (free function, static member
        // function, or functor) with the specified 'argument1' and
        // 'argument2' along with the sequence of four argument values
        // specified at construction.

  public:
    // CREATORS
    bcefi_Vfunc2F6(F                procedure,
                   const D1&        embeddedArg1,
                   const D2&        embeddedArg2,
                   const D3&        embeddedArg3,
                   const D4&        embeddedArg4,
                   bslma_Allocator *basicAllocator);
        // Create a representation for a function object (functor) taking two
        // arguments and returning 'void', using the specified 'procedure'
        // (i.e., free function, static member function, or functor) taking
        // four additional trailing arguments and the corresponding arguments'
        // specified 'embeddedArg1', 'embeddedArg2', 'embeddedArg3', and
        // 'embeddedArg4' values.  Use the specified 'basicAllocator' to
        // supply memory.  Note that any value returned from 'procedure' will
        // be ignored.
};

                        // ====================
                        // class bcefi_Vfunc2F7
                        // ====================

template <class F, class A1, class A2, class D1, class D2, class D3, class D4,
          class D5>
class bcefi_Vfunc2F7 : public bcefr_Vfunc2<A1, A2> {
    // This class defines the representation for a function object (functor),
    // characterized by a function-call operator taking two arguments and
    // returning 'void', that holds a pure procedure (i.e., free function,
    // static member function, or functor) taking five additional trailing
    // arguments, and those arguments' corresponding values.

    // DATA
    bslalg_ConstructorProxy<F>  d_f;   // function pointer (or functor)
    bslalg_ConstructorProxy<D1> d_d1;  // first embedded argument
    bslalg_ConstructorProxy<D2> d_d2;  // second embedded argument
    bslalg_ConstructorProxy<D3> d_d3;  // third embedded argument
    bslalg_ConstructorProxy<D4> d_d4;  // fourth embedded argument
    bslalg_ConstructorProxy<D5> d_d5;  // fifth embedded argument

    // NOT IMPLEMENTED
    bcefi_Vfunc2F7(const bcefi_Vfunc2F7<F, A1, A2, D1, D2, D3, D4, D5>&);
    bcefi_Vfunc2F7<F, A1, A2, D1, D2, D3, D4, D5>& operator=(
                   const bcefi_Vfunc2F7<F, A1, A2, D1, D2, D3, D4, D5>&);

  private:
    // PRIVATE CREATORS
    ~bcefi_Vfunc2F7();
        // Destroy this functor.  Note that this destructor can be invoked only
        // indirectly, via the static 'deleteObject' method of the base class.

    // PRIVATE ACCESSORS
    void execute(const A1& argument1, const A2& argument2) const;
        // Invoke the underlying procedure (free function, static member
        // function, or functor) with the specified 'argument1' and
        // 'argument2' along with the sequence of five argument values
        // specified at construction.

  public:
    // CREATORS
    bcefi_Vfunc2F7(F                procedure,
                   const D1&        embeddedArg1,
                   const D2&        embeddedArg2,
                   const D3&        embeddedArg3,
                   const D4&        embeddedArg4,
                   const D5&        embeddedArg5,
                   bslma_Allocator *basicAllocator);
        // Create a representation for a function object (functor) taking two
        // arguments and returning 'void', using the specified 'procedure'
        // (i.e., free function, static member function, or functor) taking
        // five additional trailing arguments and the corresponding arguments'
        // specified 'embeddedArg1', 'embeddedArg2', 'embeddedArg3',
        // 'embeddedArg4', and 'embeddedArg5' values.  Use the specified
        // 'basicAllocator' to supply memory.  Note that any value returned
        // from 'procedure' will be ignored.
};

                        // ====================
                        // class bcefi_Vfunc2F8
                        // ====================

template <class  F, class A1, class A2, class D1, class D2, class D3, class D4,
          class D5, class D6>
class bcefi_Vfunc2F8 : public bcefr_Vfunc2<A1, A2> {
    // This class defines the representation for a function object (functor),
    // characterized by a function-call operator taking two arguments and
    // returning 'void', that holds a pure procedure (i.e., free function,
    // static member function, or functor) taking six additional trailing
    // arguments, and those arguments' corresponding values.

    // DATA
    bslalg_ConstructorProxy<F>  d_f;   // function pointer (or functor)
    bslalg_ConstructorProxy<D1> d_d1;  // first embedded argument
    bslalg_ConstructorProxy<D2> d_d2;  // second embedded argument
    bslalg_ConstructorProxy<D3> d_d3;  // third embedded argument
    bslalg_ConstructorProxy<D4> d_d4;  // fourth embedded argument
    bslalg_ConstructorProxy<D5> d_d5;  // fifth embedded argument
    bslalg_ConstructorProxy<D6> d_d6;  // sixth embedded argument

    // NOT IMPLEMENTED
    bcefi_Vfunc2F8(const bcefi_Vfunc2F8<F, A1, A2, D1, D2, D3, D4, D5, D6>&);
    bcefi_Vfunc2F8<F, A1, A2, D1, D2, D3, D4, D5, D6>& operator=(
                   const bcefi_Vfunc2F8<F, A1, A2, D1, D2, D3, D4, D5, D6>&);

  private:
    // PRIVATE CREATORS
    ~bcefi_Vfunc2F8();
        // Destroy this functor.  Note that this destructor can be invoked only
        // indirectly, via the static 'deleteObject' method of the base class.

    // PRIVATE ACCESSORS
    void execute(const A1& argument1, const A2& argument2) const;
        // Invoke the underlying procedure (free function, static member
        // function, or functor) with the specified 'argument1' and
        // 'argument2' along with the sequence of six argument values
        // specified at construction.

  public:
    // CREATORS
    bcefi_Vfunc2F8(F                procedure,
                   const D1&        embeddedArg1,
                   const D2&        embeddedArg2,
                   const D3&        embeddedArg3,
                   const D4&        embeddedArg4,
                   const D5&        embeddedArg5,
                   const D6&        embeddedArg6,
                   bslma_Allocator *basicAllocator);
        // Create a representation for a function object (functor) taking two
        // arguments and returning 'void', using the specified 'procedure'
        // (i.e., free function, static member function, or functor) taking
        // six additional trailing arguments and the corresponding arguments'
        // specified 'embeddedArg1', 'embeddedArg2', 'embeddedArg3',
        // 'embeddedArg4', 'embeddedArg5', and 'embeddedArg6' values.  Use the
        // specified 'basicAllocator' to supply memory.  Note that any value
        // returned from 'procedure' will be ignored.
};

                        // ====================
                        // class bcefi_Vfunc2F9
                        // ====================

template <class  F, class A1, class A2, class D1, class D2, class D3, class D4,
          class D5, class D6, class D7>
class bcefi_Vfunc2F9 : public bcefr_Vfunc2<A1, A2> {
    // This class defines the representation for a function object (functor),
    // characterized by a function-call operator taking two arguments and
    // returning 'void', that holds a pure procedure (i.e., free function,
    // static member function, or functor) taking seven additional trailing
    // arguments, and those arguments' corresponding values.

    // DATA
    bslalg_ConstructorProxy<F>  d_f;   // function pointer (or functor)
    bslalg_ConstructorProxy<D1> d_d1;  // first embedded argument
    bslalg_ConstructorProxy<D2> d_d2;  // second embedded argument
    bslalg_ConstructorProxy<D3> d_d3;  // third embedded argument
    bslalg_ConstructorProxy<D4> d_d4;  // fourth embedded argument
    bslalg_ConstructorProxy<D5> d_d5;  // fifth embedded argument
    bslalg_ConstructorProxy<D6> d_d6;  // sixth embedded argument
    bslalg_ConstructorProxy<D7> d_d7;  // seventh embedded argument

    // NOT IMPLEMENTED
    bcefi_Vfunc2F9(const bcefi_Vfunc2F9<F, A1, A2, D1, D2, D3, D4, D5, D6,
                                        D7>&);
    bcefi_Vfunc2F9<F, A1, A2, D1, D2, D3, D4, D5, D6, D7>& operator=(
                   const bcefi_Vfunc2F9<F, A1, A2, D1, D2, D3, D4, D5, D6,
                                        D7>&);

  private:
    // PRIVATE CREATORS
    ~bcefi_Vfunc2F9();
        // Destroy this functor.  Note that this destructor can be invoked only
        // indirectly, via the static 'deleteObject' method of the base class.

    // PRIVATE ACCESSORS
    void execute(const A1& argument1, const A2& argument2) const;
        // Invoke the underlying procedure (free function, static member
        // function, or functor) with the specified 'argument1' and
        // 'argument2' along with the sequence of seven argument values
        // specified at construction.

  public:
    // CREATORS
    bcefi_Vfunc2F9(F                procedure,
                   const D1&        embeddedArg1,
                   const D2&        embeddedArg2,
                   const D3&        embeddedArg3,
                   const D4&        embeddedArg4,
                   const D5&        embeddedArg5,
                   const D6&        embeddedArg6,
                   const D7&        embeddedArg7,
                   bslma_Allocator *basicAllocator);
        // Create a representation for a function object (functor) taking two
        // arguments and returning 'void', using the specified 'procedure'
        // (i.e., free function, static member function, or functor) taking
        // seven additional trailing arguments and the corresponding
        // arguments' specified 'embeddedArg1', 'embeddedArg2, 'embeddedArg3',
        // 'embeddedArg4', 'embeddedArg5', 'embeddedArg6', and 'embeddedArg7'
        // values.  Use the specified 'basicAllocator' to supply memory.  Note
        // that any value returned from 'procedure' will be ignored
};

                        // ====================
                        // class bcefi_Vfunc2C2
                        // ====================

template <class OBJ, class C, class A1, class A2>
class bcefi_Vfunc2C2 : public bcefr_Vfunc2<A1, A2> {
    // This class defines the representation for a function object (functor),
    // characterized by a function-call operator taking two arguments and
    // returning void, that holds an object and one of that object's 'const'
    // member functions taking no additional arguments.

    // DATA
    bslalg_ConstructorProxy<OBJ> d_obj;  // object
    C                            d_m;    // const member function pointer

    // NOT IMPLEMENTED
    bcefi_Vfunc2C2(const bcefi_Vfunc2C2<OBJ, C, A1, A2>&);
    bcefi_Vfunc2C2<OBJ, C, A1, A2>& operator=(
                   const bcefi_Vfunc2C2<OBJ, C, A1, A2>&);

  private:
    // PRIVATE CREATORS
    ~bcefi_Vfunc2C2();
        // Destroy this functor.  Note that this destructor can be invoked only
        // indirectly, via the static 'deleteObject' method of the base class.

    // PRIVATE ACCESSORS
    void execute(const A1& argument1, const A2& argument2) const;
        // Invoke the underlying 'const' member function on the embedded object
        // with the specified 'argument1' and 'argument2' (and no additional
        // arguments).

  public:
    // CREATORS
    bcefi_Vfunc2C2(const OBJ&       object,
                   C                method,
                   bslma_Allocator *basicAllocator);
        // Create a representation for a function object (functor) taking two
        // arguments and returning 'void', using the specified 'object' and
        // the specified 'const' 'method' of 'object' taking no additional
        // arguments.  Use the specified 'basicAllocator' to supply memory.
        // Note that any value returned from 'method' will be ignored.
};

                        // ====================
                        // class bcefi_Vfunc2C3
                        // ====================

template <class OBJ, class C, class A1, class A2, class D1>
class bcefi_Vfunc2C3 : public bcefr_Vfunc2<A1, A2> {
    // This class defines the representation for a function object (functor),
    // characterized by a function-call operator taking two arguments and
    // returning 'void', that holds an object and one of that object's 'const'
    // member functions taking one additional trailing argument, and the
    // argument's corresponding value.

    // DATA
    bslalg_ConstructorProxy<OBJ> d_obj;  // object
    C                            d_m;    // const member function pointer
    bslalg_ConstructorProxy<D1>  d_d1;   // first embedded argument

    // NOT IMPLEMENTED
    bcefi_Vfunc2C3(const bcefi_Vfunc2C3<OBJ, C, A1, A2, D1>&);
    bcefi_Vfunc2C3<OBJ, C, A1, A2, D1>& operator=(
                   const bcefi_Vfunc2C3<OBJ, C, A1, A2, D1>&);

  private:
    // PRIVATE CREATORS
    ~bcefi_Vfunc2C3();
        // Destroy this functor.  Note that this destructor can be invoked only
        // indirectly, via the static 'deleteObject' method of the base class.

    // PRIVATE ACCESSORS
    void execute(const A1& argument1, const A2& argument2) const;
        // Invoke the underlying 'const' member function on the embedded object
        // with the specified 'argument1' and 'argument2' along with the
        // trailing argument value specified at construction.

  public:
    // CREATORS
    bcefi_Vfunc2C3(const OBJ&       object,
                   C                method,
                   const D1&        embeddedArg1,
                   bslma_Allocator *basicAllocator);
        // Create a representation for a function object (functor) taking two
        // arguments and returning 'void', using the specified 'object' and
        // the specified 'const' 'method' of 'object' taking one additional
        // trailing argument, and the corresponding argument's specified
        // 'embeddedArg1' value.  Use the specified 'basicAllocator' to supply
        // memory.  Note that any value returned from 'method' will be ignored.
};

                        // ====================
                        // class bcefi_Vfunc2C4
                        // ====================

template <class OBJ, class C, class A1, class A2, class D1, class D2>
class bcefi_Vfunc2C4 : public bcefr_Vfunc2<A1, A2> {
    // This class defines the representation for a function object (functor),
    // characterized by a function-call operator taking two arguments and
    // returning 'void', that holds an object and one of that object's 'const'
    // member functions taking two additional trailing arguments, and the
    // arguments' corresponding values.

    // DATA
    bslalg_ConstructorProxy<OBJ> d_obj;  // object
    C                            d_m;    // const member function pointer
    bslalg_ConstructorProxy<D1>  d_d1;   // first embedded argument
    bslalg_ConstructorProxy<D2>  d_d2;   // second embedded argument

    // NOT IMPLEMENTED
    bcefi_Vfunc2C4(const bcefi_Vfunc2C4<OBJ, C, A1, A2, D1, D2>&);
    bcefi_Vfunc2C4<OBJ, C, A1, A2, D1, D2>& operator=(
                   const bcefi_Vfunc2C4<OBJ, C, A1, A2, D1, D2>&);

  private:
    // PRIVATE CREATORS
    ~bcefi_Vfunc2C4();
        // Destroy this functor.  Note that this destructor can be invoked only
        // indirectly, via the static 'deleteObject' method of the base class.

    // PRIVATE ACCESSORS
    void execute(const A1& argument1, const A2& argument2) const;
        // Invoke the underlying 'const' member function on the embedded object
        // with the specified 'argument1' and 'argument2' along with the
        // sequence of two trailing argument values specified at construction.

  public:
    // CREATORS
    bcefi_Vfunc2C4(const OBJ&       object,
                   C                method,
                   const D1&        embeddedArg1,
                   const D2&        embeddedArg2,
                   bslma_Allocator *basicAllocator);
        // Create a representation for a function object (functor) taking two
        // arguments and returning 'void', using the specified 'object' and
        // the specified 'const' 'method' of 'object' taking two additional
        // trailing arguments, and the corresponding arguments' specified
        // 'embeddedArg1' and 'embeddedArg2' values.  Use the specified
        // 'basicAllocator' to supply memory.  Note that any value returned
        // from 'method' will be ignored.
};

                        // ====================
                        // class bcefi_Vfunc2C5
                        // ====================

template <class OBJ, class C, class A1, class A2, class D1, class D2, class D3>
class bcefi_Vfunc2C5 : public bcefr_Vfunc2<A1, A2> {
    // This class defines the representation for a function object (functor),
    // characterized by a function-call operator taking two arguments and
    // returning 'void', that holds an object and one of that object's 'const'
    // member functions taking three additional trailing arguments, and the
    // arguments' corresponding values.

    // DATA
    bslalg_ConstructorProxy<OBJ> d_obj;  // object
    C                            d_m;    // const member function pointer
    bslalg_ConstructorProxy<D1>  d_d1;   // first embedded argument
    bslalg_ConstructorProxy<D2>  d_d2;   // second embedded argument
    bslalg_ConstructorProxy<D3>  d_d3;   // third embedded argument

    // NOT IMPLEMENTED
    bcefi_Vfunc2C5(const bcefi_Vfunc2C5<OBJ, C, A1, A2, D1, D2, D3>&);
    bcefi_Vfunc2C5<OBJ, C, A1, A2, D1, D2, D3>& operator=(
                   const bcefi_Vfunc2C5<OBJ, C, A1, A2, D1, D2, D3>&);

  private:
    // PRIVATE CREATORS
    ~bcefi_Vfunc2C5();
        // Destroy this functor.  Note that this destructor can be invoked only
        // indirectly, via the static 'deleteObject' method of the base class.

    // PRIVATE ACCESSORS
    void execute(const A1& argument1, const A2& argument2) const;
        // Invoke the underlying 'const' member function on the embedded object
        // with the specified 'argument1' and 'argument2' along with the
        // sequence of three trailing argument values specified at
        // construction.

  public:
    // CREATORS
    bcefi_Vfunc2C5(const OBJ&       object,
                   C                method,
                   const D1&        embeddedArg1,
                   const D2&        embeddedArg2,
                   const D3&        embeddedArg3,
                   bslma_Allocator *basicAllocator);
        // Create a representation for a function object (functor) taking two
        // arguments and returning 'void', using the specified 'object' and
        // the specified 'const' 'method' of 'object' taking three additional
        // trailing arguments, and the corresponding arguments' specified
        // 'embeddedArg1', 'embeddedArg2', and 'embeddedArg3' values.  Use the
        // specified 'basicAllocator' to supply memory.  Note that any value
        // returned from 'method' will be ignored.
};

                        // ====================
                        // class bcefi_Vfunc2C6
                        // ====================

template <class OBJ, class C, class A1, class A2, class D1, class D2, class D3,
          class D4>
class bcefi_Vfunc2C6 : public bcefr_Vfunc2<A1, A2> {
    // This class defines the representation for a function object (functor),
    // characterized by a function-call operator taking two arguments and
    // returning 'void', that holds an object and one of that object's 'const'
    // member functions taking four additional trailing arguments, and the
    // arguments' corresponding values.

    // DATA
    bslalg_ConstructorProxy<OBJ> d_obj;  // object
    C                            d_m;    // const member function pointer
    bslalg_ConstructorProxy<D1>  d_d1;   // first embedded argument
    bslalg_ConstructorProxy<D2>  d_d2;   // second embedded argument
    bslalg_ConstructorProxy<D3>  d_d3;   // third embedded argument
    bslalg_ConstructorProxy<D4>  d_d4;   // fourth embedded argument

    // NOT IMPLEMENTED
    bcefi_Vfunc2C6(const bcefi_Vfunc2C6<OBJ, C, A1, A2, D1, D2, D3, D4>&);
    bcefi_Vfunc2C6<OBJ, C, A1, A2, D1, D2, D3, D4>& operator=(
                   const bcefi_Vfunc2C6<OBJ, C, A1, A2, D1, D2, D3, D4>&);

  private:
    // PRIVATE CREATORS
    ~bcefi_Vfunc2C6();
        // Destroy this functor.  Note that this destructor can be invoked only
        // indirectly, via the static 'deleteObject' method of the base class.

    // PRIVATE ACCESSORS
    void execute(const A1& argument1, const A2& argument2) const;
        // Invoke the underlying 'const' member function on the embedded object
        // with the specified 'argument1' and 'argument2' along with the
        // sequence of four trailing argument values specified at construction.

  public:
    // CREATORS
    bcefi_Vfunc2C6(const OBJ&       object,
                   C                method,
                   const D1&        embeddedArg1,
                   const D2&        embeddedArg2,
                   const D3&        embeddedArg3,
                   const D4&        embeddedArg4,
                   bslma_Allocator *basicAllocator);
        // Create a representation for a function object (functor) taking two
        // arguments and returning 'void', using the specified 'object' and
        // the specified 'const' 'method' of 'object' taking four additional
        // trailing arguments, and the corresponding arguments' specified
        // 'embeddedArg1', 'embeddedArg2', 'embeddedArg3', and 'embeddedArg4'
        // values.  Use the specified 'basicAllocator' to supply memory.  Note
        // that any value returned from 'method' will be ignored.
};

                        // ====================
                        // class bcefi_Vfunc2C7
                        // ====================

template <class OBJ, class C, class A1, class A2, class D1, class D2, class D3,
          class D4, class D5>
class bcefi_Vfunc2C7 : public bcefr_Vfunc2<A1, A2> {
    // This class defines the representation for a function object (functor),
    // characterized by a function-call operator taking two arguments and
    // returning 'void', that holds an object and one of that object's 'const'
    // member functions taking five additional trailing arguments, and the
    // arguments' corresponding values.

    // DATA
    bslalg_ConstructorProxy<OBJ> d_obj;  // object
    C                            d_m;    // const member function pointer
    bslalg_ConstructorProxy<D1>  d_d1;   // first embedded argument
    bslalg_ConstructorProxy<D2>  d_d2;   // second embedded argument
    bslalg_ConstructorProxy<D3>  d_d3;   // third embedded argument
    bslalg_ConstructorProxy<D4>  d_d4;   // fourth embedded argument
    bslalg_ConstructorProxy<D5>  d_d5;   // fifth embedded argument

    // NOT IMPLEMENTED
    bcefi_Vfunc2C7(const bcefi_Vfunc2C7<OBJ, C, A1, A2, D1, D2, D3, D4, D5>&);
    bcefi_Vfunc2C7<OBJ, C, A1, A2, D1, D2, D3, D4, D5>& operator=(
                   const bcefi_Vfunc2C7<OBJ, C, A1, A2, D1, D2, D3, D4, D5>&);

  private:
    // PRIVATE CREATORS
    ~bcefi_Vfunc2C7();
        // Destroy this functor.  Note that this destructor can be invoked only
        // indirectly, via the static 'deleteObject' method of the base class.

    // PRIVATE ACCESSORS
    void execute(const A1& argument1, const A2& argument2) const;
        // Invoke the underlying 'const' member function on the embedded object
        // with the specified 'argument1' and 'argument2' along with the
        // sequence of five trailing argument values specified at construction.

  public:
    // CREATORS
    bcefi_Vfunc2C7(const OBJ&       object,
                   C                method,
                   const D1&        embeddedArg1,
                   const D2&        embeddedArg2,
                   const D3&        embeddedArg3,
                   const D4&        embeddedArg4,
                   const D5&        embeddedArg5,
                   bslma_Allocator *basicAllocator);
        // Create a representation for a function object (functor) taking two
        // arguments and returning 'void', using the specified 'object' and
        // the specified 'const' 'method' of 'object' taking five additional
        // trailing arguments, and the corresponding arguments' specified
        // 'embeddedArg1', 'embeddedArg2', 'embeddedArg3', 'embeddedArg4', and
        // 'embeddedArg5' values.  Use the specified 'basicAllocator' to
        // supply memory.  Note that any value returned from 'method' will be
        // ignored.
};

                        // ====================
                        // class bcefi_Vfunc2C8
                        // ====================

template <class OBJ, class C, class A1, class A2, class D1, class D2, class D3,
          class D4, class D5, class D6>
class bcefi_Vfunc2C8 : public bcefr_Vfunc2<A1, A2> {
    // This class defines the representation for a function object (functor),
    // characterized by a function-call operator taking two arguments and
    // returning 'void', that holds an object and one of that object's 'const'
    // member functions taking six additional trailing arguments, and the
    // arguments' corresponding values.

    // DATA
    bslalg_ConstructorProxy<OBJ>                         d_obj;  // object
    C                           d_m;    // const member function pointer
    bslalg_ConstructorProxy<D1> d_d1;   // first embedded argument
    bslalg_ConstructorProxy<D2> d_d2;   // second embedded argument
    bslalg_ConstructorProxy<D3> d_d3;   // third embedded argument
    bslalg_ConstructorProxy<D4> d_d4;   // fourth embedded argument
    bslalg_ConstructorProxy<D5> d_d5;   // fifth embedded argument
    bslalg_ConstructorProxy<D6> d_d6;   // sixth embedded argument

    // NOT IMPLEMENTED
    bcefi_Vfunc2C8(const bcefi_Vfunc2C8<OBJ, C, A1, A2, D1, D2, D3, D4, D5,
                                        D6>&);
    bcefi_Vfunc2C8<OBJ, C, A1, A2, D1, D2, D3, D4, D5, D6>& operator=(
                   const bcefi_Vfunc2C8<OBJ, C, A1, A2, D1, D2, D3, D4, D5,
                                        D6>&);

  private:
    // PRIVATE CREATORS
    ~bcefi_Vfunc2C8();
        // Destroy this functor.  Note that this destructor can be invoked only
        // indirectly, via the static 'deleteObject' method of the base class.

    // PRIVATE ACCESSORS
    void execute(const A1& argument1, const A2& argument2) const;
        // Invoke the underlying 'const' member function on the embedded object
        // with the specified 'argument1' and 'argument2' along with the
        // sequence of six trailing argument values specified at construction.

  public:
    // CREATORS
    bcefi_Vfunc2C8(const OBJ&       object,
                   C                method,
                   const D1&        embeddedArg1,
                   const D2&        embeddedArg2,
                   const D3&        embeddedArg3,
                   const D4&        embeddedArg4,
                   const D5&        embeddedArg5,
                   const D6&        embeddedArg6,
                   bslma_Allocator *basicAllocator);
        // Create a representation for a function object (functor) taking two
        // arguments and returning 'void', using the specified 'object' and
        // the specified 'const' 'method' of 'object' taking six additional
        // trailing arguments, and the corresponding arguments' specified
        // 'embeddedArg1', 'embeddedArg2', 'embeddedArg3', 'embeddedArg4',
        // 'embeddedArg5', and 'embeddedArg6' values.  Use the specified
        // 'basicAllocator' to supply memory.  Note that any value returned
        // from 'method' will be ignored.
};

                        // ====================
                        // class bcefi_Vfunc2C9
                        // ====================

template <class OBJ, class C, class A1, class A2, class D1, class D2, class D3,
          class D4, class D5, class D6, class D7>
class bcefi_Vfunc2C9 : public bcefr_Vfunc2<A1, A2> {
    // This class defines the representation for a function object (functor),
    // characterized by a function-call operator taking two arguments and
    // returning 'void', that holds an object and one of that object's 'const'
    // member functions taking seven additional trailing arguments, and the
    // arguments' corresponding values.

    // DATA
    bslalg_ConstructorProxy<OBJ>                         d_obj;  // object
    C                           d_m;    // const member function pointer
    bslalg_ConstructorProxy<D1> d_d1;   // first embedded argument
    bslalg_ConstructorProxy<D2> d_d2;   // second embedded argument
    bslalg_ConstructorProxy<D3> d_d3;   // third embedded argument
    bslalg_ConstructorProxy<D4> d_d4;   // fourth embedded argument
    bslalg_ConstructorProxy<D5> d_d5;   // fifth embedded argument
    bslalg_ConstructorProxy<D6> d_d6;   // sixth embedded argument
    bslalg_ConstructorProxy<D7> d_d7;   // seventh embedded argument

    // NOT IMPLEMENTED
    bcefi_Vfunc2C9(const bcefi_Vfunc2C9<OBJ, C, A1, A2, D1, D2, D3, D4, D5,
                                        D6, D7>&);
    bcefi_Vfunc2C9<OBJ, C, A1, A2, D1, D2, D3, D4, D5, D6, D7>& operator=(
                   const bcefi_Vfunc2C9<OBJ, C, A1, A2, D1, D2, D3, D4, D5,
                                        D6, D7>&);

  private:
    // PRIVATE CREATORS
    ~bcefi_Vfunc2C9();
        // Destroy this functor.  Note that this destructor can be invoked only
        // indirectly, via the static 'deleteObject' method of the base class.

    // PRIVATE ACCESSORS
    void execute(const A1& argument1, const A2& argument2) const;
        // Invoke the underlying 'const' member function on the embedded object
        // with the specified 'argument1' and 'argument2' along with the
        // sequence of seven trailing argument values specified at
        // construction.

  public:
    // CREATORS
    bcefi_Vfunc2C9(const OBJ&       object,
                   C                method,
                   const D1&        embeddedArg1,
                   const D2&        embeddedArg2,
                   const D3&        embeddedArg3,
                   const D4&        embeddedArg4,
                   const D5&        embeddedArg5,
                   const D6&        embeddedArg6,
                   const D7&        embeddedArg7,
                   bslma_Allocator *basicAllocator);
        // Create a representation for a function object (functor) taking two
        // arguments and returning 'void', using the specified 'object' and
        // the specified 'const' 'method' of 'object' taking seven additional
        // trailing arguments, and the corresponding arguments' specified
        // 'embeddedArg1', 'embeddedArg2', 'embeddedArg3', 'embeddedArg4',
        // 'embeddedArg5', 'embeddedArg6', and 'embeddedArg7' values.  Use the
        // specified 'basicAllocator' to supply memory.  Note that any value
        // returned from 'method' will be ignored.
};

                        // ====================
                        // class bcefi_Vfunc2M2
                        // ====================

template <class OBJ, class M, class A1, class A2>
class bcefi_Vfunc2M2 : public bcefr_Vfunc2<A1, A2> {
    // This class defines the representation for a function object (functor),
    // characterized by a function-call operator taking two arguments and
    // returning 'void', that holds the address of an object and one of that
    // object's ('const' or non-'const') member functions taking no additional
    // arguments.

    // DATA
    OBJ *d_obj_p;  // object
    M    d_m;      // member function pointer

    // NOT IMPLEMENTED
    bcefi_Vfunc2M2(const bcefi_Vfunc2M2<OBJ, M, A1, A2>&);
    bcefi_Vfunc2M2<OBJ, M, A1, A2>& operator=(
                   const bcefi_Vfunc2M2<OBJ, M, A1, A2>&);

  private:
    // PRIVATE CREATORS
    ~bcefi_Vfunc2M2();
        // Destroy this functor.  Note that this destructor can be invoked only
        // indirectly, via the static 'deleteObject' method of the base class.

    // PRIVATE ACCESSORS
    void execute(const A1& argument1, const A2& argument2) const;
        // Invoke the underlying member function on the embedded object pointer
        // with the specified 'argument1' and 'argument2' (and no additional
        // arguments).

  public:
    // CREATORS
    bcefi_Vfunc2M2(OBJ             *object,
                   M                method,
                   bslma_Allocator *basicAllocator);
        // Create a representation for a function object (functor) taking two
        // arguments and returning 'void', using the specified 'object'
        // address and the specified ('const' or non-'const') 'method' of
        // 'object' taking no additional arguments.  Use the specified
        // 'basicAllocator' to supply memory.  Note that any value returned
        // from 'method' will be ignored.
};

                        // ====================
                        // class bcefi_Vfunc2M3
                        // ====================

template <class OBJ, class M, class A1, class A2, class D1>
class bcefi_Vfunc2M3 : public bcefr_Vfunc2<A1, A2> {
    // This class defines the representation for a function object (functor),
    // characterized by a function-call operator taking two arguments and
    // returning 'void', that holds the address of an object and one of that
    // object's ('const' or non-'const') member functions taking one
    // additional trailing argument, and that argument's corresponding value.

    // DATA
    OBJ                         *d_obj_p;  // object
    M                            d_m;      // member function pointer
    bslalg_ConstructorProxy<D1>  d_d1;     // first embedded argument

    // NOT IMPLEMENTED
    bcefi_Vfunc2M3(const bcefi_Vfunc2M3<OBJ, M, A1, A2, D1>&);
    bcefi_Vfunc2M3<OBJ, M, A1, A2, D1>& operator=(
                   const bcefi_Vfunc2M3<OBJ, M, A1, A2, D1>&);

  private:
    // PRIVATE CREATORS
    ~bcefi_Vfunc2M3();
        // Destroy this functor.  Note that this destructor can be invoked only
        // indirectly, via the static 'deleteObject' method of the base class.

    // PRIVATE ACCESSORS
    void execute(const A1& argument1, const A2& argument2) const;
        // Invoke the underlying member function on the embedded object pointer
        // with the specified 'argument1' and 'argument2' along with the
        // additional trailing argument value specified at construction.

  public:
    // CREATORS
    bcefi_Vfunc2M3(OBJ             *object,
                   M                method,
                   const D1&        embeddedArg1,
                   bslma_Allocator *basicAllocator);
        // Create a representation for a function object (functor) taking two
        // arguments and returning 'void', using the specified 'object'
        // address and the specified ('const' or non-'const') 'method' of
        // 'object' taking one additional trailing argument, and the
        // corresponding argument's specified 'embeddedArg1' value.  Use the
        // specified 'basicAllocator' to supply memory.  Note that any value
        // returned from 'method' will be ignored.
};

                        // ====================
                        // class bcefi_Vfunc2M4
                        // ====================

template <class OBJ, class M, class A1, class A2, class D1, class D2>
class bcefi_Vfunc2M4 : public bcefr_Vfunc2<A1, A2> {
    // This class defines the representation for a function object (functor),
    // characterized by a function-call operator taking two arguments and
    // returning 'void', that holds the address of an object and one of that
    // object's ('const' or non-'const') member functions taking two
    // additional trailing arguments, and those arguments' corresponding
    // values.

    // DATA
    OBJ                         *d_obj_p;  // object
    M                            d_m;      // member function pointer
    bslalg_ConstructorProxy<D1>  d_d1;     // first embedded argument
    bslalg_ConstructorProxy<D2>  d_d2;     // second embedded argument

    // NOT IMPLEMENTED
    bcefi_Vfunc2M4(const bcefi_Vfunc2M4<OBJ, M, A1, A2, D1, D2>&);
    bcefi_Vfunc2M4<OBJ, M, A1, A2, D1, D2>& operator=(
                   const bcefi_Vfunc2M4<OBJ, M, A1, A2, D1, D2>&);

  private:
    // PRIVATE CREATORS
    ~bcefi_Vfunc2M4();
        // Destroy this functor.  Note that this destructor can be invoked only
        // indirectly, via the static 'deleteObject' method of the base class.

    // PRIVATE ACCESSORS
    void execute(const A1& argument1, const A2& argument2) const;
        // Invoke the underlying member function on the embedded object pointer
        // with the specified 'argument1' and 'argument2' along with the
        // sequence of two argument values specified at construction.

  public:
    // CREATORS
    bcefi_Vfunc2M4(OBJ             *object,
                   M                method,
                   const D1&        embeddedArg1,
                   const D2&        embeddedArg2,
                   bslma_Allocator *basicAllocator);
        // Create a representation for a function object (functor) taking two
        // arguments and returning 'void', using the specified 'object'
        // address and the specified ('const' or non-'const') 'method' of
        // 'object' taking two additional trailing arguments, and the
        // corresponding arguments' specified 'embeddedArg1' and
        // 'embeddedArg2' values.  Use the specified 'basicAllocator' to
        // supply memory.  Note that any value returned from 'method' will be
        // ignored.
};
                        // ====================
                        // class bcefi_Vfunc2M5
                        // ====================

template <class OBJ, class M, class A1, class A2, class D1, class D2, class D3>
class bcefi_Vfunc2M5 : public bcefr_Vfunc2<A1, A2> {
    // This class defines the representation for a function object (functor),
    // characterized by a function-call operator taking two arguments and
    // returning 'void', that holds the address of an object and one of that
    // object's ('const' or non-'const') member functions taking three
    // additional trailing arguments, and those arguments' corresponding
    // values.

    // DATA
    OBJ                         *d_obj_p;  // object
    M                            d_m;      // member function pointer
    bslalg_ConstructorProxy<D1>  d_d1;     // first embedded argument
    bslalg_ConstructorProxy<D2>  d_d2;     // second embedded argument
    bslalg_ConstructorProxy<D3>  d_d3;     // third embedded argument

    // NOT IMPLEMENTED
    bcefi_Vfunc2M5(const bcefi_Vfunc2M5<OBJ, M, A1, A2, D1, D2, D3>&);
    bcefi_Vfunc2M5<OBJ, M, A1, A2, D1, D2, D3>& operator=(
                   const bcefi_Vfunc2M5<OBJ, M, A1, A2, D1, D2, D3>&);

  private:
    // PRIVATE CREATORS
    ~bcefi_Vfunc2M5();
        // Destroy this functor.  Note that this destructor can be invoked only
        // indirectly, via the static 'deleteObject' method of the base class.

    // PRIVATE ACCESSORS
    void execute(const A1& argument, const A2& argument2) const;
        // Invoke the underlying member function on the embedded object pointer
        // with the specified 'argument1' and 'argument2' along with the
        // sequence of three argument values specified at construction.

  public:
    // CREATORS
    bcefi_Vfunc2M5(OBJ             *object,
                   M                method,
                   const D1&        embeddedArg1,
                   const D2&        embeddedArg2,
                   const D3&        embeddedArg3,
                   bslma_Allocator *basicAllocator);
        // Create a representation for a function object (functor) taking two
        // arguments and returning void, using the specified object address
        // and the specified ('const' or non-'const') 'method' of 'object'
        // taking three additional trailing arguments, and the corresponding
        // arguments' specified 'embeddedArg1', 'embeddedArg2', and
        // 'embeddedArg3' values.  Use the specified 'basicAllocator' to
        // supply memory.  Note that any value returned from 'method' will be
        // ignored.
};

                        // ====================
                        // class bcefi_Vfunc2M6
                        // ====================

template <class OBJ, class M, class A1, class A2, class D1, class D2, class D3,
          class D4>
class bcefi_Vfunc2M6 : public bcefr_Vfunc2<A1, A2> {
    // This class defines the representation for a function object (functor),
    // characterized by a function-call operator taking two arguments and
    // returning 'void', that holds the address of an object and one of that
    // object's ('const' or non-'const') member functions taking four
    // additional trailing arguments, and those arguments' corresponding
    // values.

    // DATA
    OBJ                         *d_obj_p;  // object
    M                            d_m;      // member function pointer
    bslalg_ConstructorProxy<D1>  d_d1;     // first embedded argument
    bslalg_ConstructorProxy<D2>  d_d2;     // second embedded argument
    bslalg_ConstructorProxy<D3>  d_d3;     // third embedded argument
    bslalg_ConstructorProxy<D4>  d_d4;     // fourth embedded argument

    // NOT IMPLEMENTED
    bcefi_Vfunc2M6(const bcefi_Vfunc2M6<OBJ, M, A1, A2, D1, D2, D3, D4>&);
    bcefi_Vfunc2M6<OBJ, M, A1, A2, D1, D2, D3, D4>& operator=(
                   const bcefi_Vfunc2M6<OBJ, M, A1, A2, D1, D2, D3, D4>&);

  private:
    // PRIVATE CREATORS
    ~bcefi_Vfunc2M6();
        // Destroy this functor.  Note that this destructor can be invoked only
        // indirectly, via the static 'deleteObject' method of the base class.

    // PRIVATE ACCESSORS
    void execute(const A1& argument1, const A2& argument2) const;
        // Invoke the underlying 'const' member function on the embedded object
        // with the specified 'argument1' and 'argument2' along with the
        // sequence of four trailing argument values specified at construction.

  public:
    // CREATORS
    bcefi_Vfunc2M6(OBJ             *object,
                   M                method,
                   const D1&        embeddedArg1,
                   const D2&        embeddedArg2,
                   const D3&        embeddedArg3,
                   const D4&        embeddedArg4,
                   bslma_Allocator *basicAllocator);
        // Create a representation for a function object (functor) taking two
        // arguments and returning 'void', using the specified 'object'
        // address and the specified ('const' or non-'const') 'method' of
        // 'object' 'method' taking four additional trailing arguments, and
        // the corresponding arguments' specified 'embeddedArg1',
        // 'embeddedArg2', 'embeddedArg3', and 'embeddedArg4' values.  Use the
        // specified 'basicAllocator' to supply memory.  Note that any value
        // returned from 'method' will be ignored.
};

                        // ====================
                        // class bcefi_Vfunc2M7
                        // ====================

template <class OBJ, class M, class A1, class A2, class D1, class D2, class D3,
          class D4, class D5>
class bcefi_Vfunc2M7 : public bcefr_Vfunc2<A1, A2> {
    // This class defines the representation for a function object (functor),
    // characterized by a function-call operator taking two arguments and
    // returning 'void', that holds the address of an object and one of that
    // object's ('const' or non-'const') member functions taking five
    // additional trailing arguments, and those arguments' corresponding
    // values.

    // DATA
    OBJ                         *d_obj_p;  // object
    M                            d_m;      // member function pointer
    bslalg_ConstructorProxy<D1>  d_d1;     // first embedded argument
    bslalg_ConstructorProxy<D2>  d_d2;     // second embedded argument
    bslalg_ConstructorProxy<D3>  d_d3;     // third embedded argument
    bslalg_ConstructorProxy<D4>  d_d4;     // fourth embedded argument
    bslalg_ConstructorProxy<D5>  d_d5;     // fifth embedded argument

    // NOT IMPLEMENTED
    bcefi_Vfunc2M7(const bcefi_Vfunc2M7<OBJ, M, A1, A2, D1, D2, D3, D4, D5>&);
    bcefi_Vfunc2M7<OBJ, M, A1, A2, D1, D2, D3, D4, D5>& operator=(
                   const bcefi_Vfunc2M7<OBJ, M, A1, A2, D1, D2, D3, D4, D5>&);

  private:
    // PRIVATE CREATORS
    ~bcefi_Vfunc2M7();
        // Destroy this functor.  Note that this destructor can be invoked only
        // indirectly, via the static 'deleteObject' method of the base class.

    // PRIVATE ACCESSORS
    void execute(const A1& argument1, const A2& argument2) const;
        // Invoke the underlying 'const' member function on the embedded object
        // with the specified 'argument1' and 'argument2' along with the
        // sequence of five trailing argument values specified at construction.

  public:
    // CREATORS
    bcefi_Vfunc2M7(OBJ             *object,
                   M                method,
                   const D1&        embeddedArg1,
                   const D2&        embeddedArg2,
                   const D3&        embeddedArg3,
                   const D4&        embeddedArg4,
                   const D5&        embeddedArg5,
                   bslma_Allocator *basicAllocator);
        // Create a representation for a function object (functor) taking two
        // arguments and returning 'void', using the specified 'object'
        // address and the specified ('const' or non-'const') 'method' of
        // 'object' taking five additional trailing arguments, and the
        // corresponding arguments' specified 'embeddedArg1', 'embeddedArg2',
        // 'embeddedArg3', 'embeddedArg4', and 'embeddedArg5' values.  Use the
        // specified 'basicAllocator' to supply memory.  Note that any value
        // returned from 'method' will be ignored.
};

                        // ====================
                        // class bcefi_Vfunc2M8
                        // ====================

template <class OBJ, class M, class A1, class A2, class D1, class D2, class D3,
          class D4, class D5, class D6>
class bcefi_Vfunc2M8 : public bcefr_Vfunc2<A1, A2> {
    // This class defines the representation for a function object (functor),
    // characterized by a function-call operator taking two arguments and
    // returning 'void', that holds the address of an object and one of that
    // object's ('const' or non-'const') member functions taking six
    // additional trailing arguments, and those arguments' corresponding
    // values.

    // DATA
    OBJ                         *d_obj_p;  // object
    M                            d_m;      // member function pointer
    bslalg_ConstructorProxy<D1>  d_d1;     // first embedded argument
    bslalg_ConstructorProxy<D2>  d_d2;     // second embedded argument
    bslalg_ConstructorProxy<D3>  d_d3;     // third embedded argument
    bslalg_ConstructorProxy<D4>  d_d4;     // fourth embedded argument
    bslalg_ConstructorProxy<D5>  d_d5;     // fifth embedded argument
    bslalg_ConstructorProxy<D6>  d_d6;     // sixth embedded argument

    // NOT IMPLEMENTED
    bcefi_Vfunc2M8(const bcefi_Vfunc2M8<OBJ, M, A1, A2, D1, D2, D3, D4, D5,
                                        D6>&);
    bcefi_Vfunc2M8<OBJ, M, A1, A2, D1, D2, D3, D4, D5, D6>& operator=(
                   const bcefi_Vfunc2M8<OBJ, M, A1, A2, D1, D2, D3, D4, D5,
                                        D6>&);

  private:
    // PRIVATE CREATORS
    ~bcefi_Vfunc2M8();
        // Destroy this functor.  Note that this destructor can be invoked only
        // indirectly, via the static 'deleteObject' method of the base class.

    // PRIVATE ACCESSORS
    void execute(const A1& argument1, const A2& argument2) const;
        // Invoke the underlying 'const' member function on the embedded object
        // with the specified 'argument1' and 'argument2' along with the
        // sequence of six trailing argument values specified at construction.

  public:
    // CREATORS
    bcefi_Vfunc2M8(OBJ             *object,
                   M                method,
                   const D1&        embeddedArg1,
                   const D2&        embeddedArg2,
                   const D3&        embeddedArg3,
                   const D4&        embeddedArg4,
                   const D5&        embeddedArg5,
                   const D6&        embeddedArg6,
                   bslma_Allocator *basicAllocator);
        // Create a representation for a function object (functor) taking two
        // arguments and returning 'void', using the specified 'object'
        // address and the specified ('const' or non-'const') 'method' of
        // 'object' taking six additional trailing arguments, and the
        // corresponding arguments' specified 'embeddedArg1', 'embeddedArg2',
        // 'embeddedArg3', 'embeddedArg4', 'embeddedArg5', and 'embeddedArg6'
        // values.  Use the specified 'basicAllocator' to supply memory.  Note
        // that any value returned from 'method' will be ignored.
};

                        // ====================
                        // class bcefi_Vfunc2M9
                        // ====================

template <class OBJ, class M, class A1, class A2, class D1, class D2, class D3,
          class D4, class D5, class D6, class D7>
class bcefi_Vfunc2M9 : public bcefr_Vfunc2<A1, A2> {
    // This class defines the representation for a function object (functor),
    // characterized by a function-call operator taking two arguments and
    // returning 'void', that holds the address of an object and one of that
    // object's ('const' or non-'const') member functions taking seven
    // additional trailing arguments, and those arguments' corresponding
    // values.

    // DATA
    OBJ                         *d_obj_p;  // object
    M                            d_m;      // member function pointer
    bslalg_ConstructorProxy<D1>  d_d1;     // first embedded argument
    bslalg_ConstructorProxy<D2>  d_d2;     // second embedded argument
    bslalg_ConstructorProxy<D3>  d_d3;     // third embedded argument
    bslalg_ConstructorProxy<D4>  d_d4;     // fourth embedded argument
    bslalg_ConstructorProxy<D5>  d_d5;     // fifth embedded argument
    bslalg_ConstructorProxy<D6>  d_d6;     // sixth embedded argument
    bslalg_ConstructorProxy<D7>  d_d7;     // seventh embedded argument

    // NOT IMPLEMENTED
    bcefi_Vfunc2M9(const bcefi_Vfunc2M9<OBJ, M, A1, A2, D1, D2, D3, D4, D5,
                                        D6, D7>&);
    bcefi_Vfunc2M9<OBJ, M, A1, A2, D1, D2, D3, D4, D5, D6, D7>& operator=(
                   const bcefi_Vfunc2M9<OBJ, M, A1, A2, D1, D2, D3, D4, D5,
                                        D6, D7>&);

  private:
    // PRIVATE CREATORS
    ~bcefi_Vfunc2M9();
        // Destroy this functor.  Note that this destructor can be invoked only
        // indirectly, via the static 'deleteObject' method of the base class.

    // PRIVATE ACCESSORS
    void execute(const A1& argument1, const A2& argument2) const;
        // Invoke the underlying 'const' member function on the embedded object
        // with the specified 'argument1' and 'argument2' along with the
        // sequence of seven trailing argument values specified at
        // construction.

  public:
    // CREATORS
    bcefi_Vfunc2M9(OBJ             *object,
                   M                method,
                   const D1&        embeddedArg1,
                   const D2&        embeddedArg2,
                   const D3&        embeddedArg3,
                   const D4&        embeddedArg4,
                   const D5&        embeddedArg5,
                   const D6&        embeddedArg6,
                   const D7&        embeddedArg7,
                   bslma_Allocator *basicAllocator);
        // Create a representation for a function object (functor) taking two
        // arguments and returning 'void', using the specified 'object'
        // address and the specified ('const' or non-'const') 'method' of
        // 'object' 'method' taking seven additional trailing arguments, and
        // the corresponding arguments' specified 'embeddedArg1',
        // 'embeddedArg2', 'embeddedArg3', 'embeddedArg4', 'embeddedArg5',
        // 'embeddedArg6', and 'embeddedArg7' values.  Use the specified
        // 'basicAllocator' to supply memory.  Note that any value returned
        // from 'method' will be ignored.
};

// ============================================================================
//                      INLINE FUNCTION DEFINITIONS
// ============================================================================

                        // ----------------------
                        // class bcefi_Vfunc2Null
                        // ----------------------

template <class A1, class A2>
inline
bcefi_Vfunc2Null<A1, A2>::bcefi_Vfunc2Null(bslma_Allocator *basicAllocator)
: bcefr_Vfunc2<A1, A2>(basicAllocator)
{
}

template <class A1, class A2>
inline
bcefi_Vfunc2Null<A1, A2>::~bcefi_Vfunc2Null()
{
}

template <class A1, class A2>
inline
void bcefi_Vfunc2Null<A1, A2>::execute(const A1&, const A2&) const
{
}

                        // --------------------
                        // class bcefi_Vfunc2F2
                        // --------------------

template <class F, class A1, class A2>
inline
bcefi_Vfunc2F2<F, A1, A2>::bcefi_Vfunc2F2(F                procedure,
                                          bslma_Allocator *basicAllocator)
: bcefr_Vfunc2<A1, A2>(basicAllocator)
, d_f(procedure, basicAllocator)
{
}

template <class F, class A1, class A2>
inline
bcefi_Vfunc2F2<F, A1, A2>::~bcefi_Vfunc2F2()
{
}

template <class F, class A1, class A2>
inline
void bcefi_Vfunc2F2<F, A1, A2>::execute(const A1& argument1,
                                        const A2& argument2) const
{
    const F& f = d_f.object();  // for readability
    f(argument1, argument2);
}

                        // --------------------
                        // class bcefi_Vfunc2F3
                        // --------------------

template <class F, class A1, class A2, class D1>
inline
bcefi_Vfunc2F3<F, A1, A2, D1>::bcefi_Vfunc2F3(F                procedure,
                                              const D1&        embeddedArg1,
                                              bslma_Allocator *basicAllocator)
: bcefr_Vfunc2<A1, A2>(basicAllocator)
, d_f(procedure, basicAllocator)
, d_d1(embeddedArg1, basicAllocator)
{
}

template <class F, class A1, class A2, class D1>
inline
bcefi_Vfunc2F3<F, A1, A2, D1>::~bcefi_Vfunc2F3()
{
}

template <class F, class A1, class A2, class D1>
inline
void bcefi_Vfunc2F3<F, A1, A2, D1>::execute(const A1& argument1,
                                            const A2& argument2) const
{
    const F& f = d_f.object();  // for readability
    f(argument1, argument2, d_d1.object());
}

                        // --------------------
                        // class bcefi_Vfunc2F4
                        // --------------------

template <class F, class A1, class A2, class D1, class D2>
inline
bcefi_Vfunc2F4<F, A1, A2, D1, D2>::bcefi_Vfunc2F4(
                                             F                procedure,
                                             const D1&        embeddedArg1,
                                             const D2&        embeddedArg2,
                                             bslma_Allocator *basicAllocator)
: bcefr_Vfunc2<A1, A2>(basicAllocator)
, d_f(procedure, basicAllocator)
, d_d1(embeddedArg1, basicAllocator)
, d_d2(embeddedArg2, basicAllocator)
{
}

template <class F, class A1, class A2, class D1, class D2>
inline
bcefi_Vfunc2F4<F, A1, A2, D1, D2>::~bcefi_Vfunc2F4()
{
}

template <class F, class A1, class A2, class D1, class D2>
inline
void bcefi_Vfunc2F4<F, A1, A2, D1, D2>::execute(const A1& argument1,
                                                const A2& argument2) const
{
    const F& f = d_f.object();  // for readability
    f(argument1, argument2, d_d1.object(), d_d2.object());
}

                        // --------------------
                        // class bcefi_Vfunc2F5
                        // --------------------

template <class F, class A1, class A2, class D1, class D2, class D3>
inline
bcefi_Vfunc2F5<F, A1, A2, D1, D2, D3>::bcefi_Vfunc2F5(
                                             F                procedure,
                                             const D1&        embeddedArg1,
                                             const D2&        embeddedArg2,
                                             const D3&        embeddedArg3,
                                             bslma_Allocator *basicAllocator)
: bcefr_Vfunc2<A1, A2>(basicAllocator)
, d_f(procedure, basicAllocator)
, d_d1(embeddedArg1, basicAllocator)
, d_d2(embeddedArg2, basicAllocator)
, d_d3(embeddedArg3, basicAllocator)
{
}

template <class F, class A1, class A2, class D1, class D2, class D3>
inline
bcefi_Vfunc2F5<F, A1, A2, D1, D2, D3>::~bcefi_Vfunc2F5()
{
}

template <class F, class A1, class A2, class D1, class D2, class D3>
inline
void bcefi_Vfunc2F5<F, A1, A2, D1, D2, D3>::execute(const A1& argument1,
                                                    const A2& argument2) const
{
    const F& f = d_f.object();  // for readability
    f(argument1, argument2, d_d1.object(), d_d2.object(), d_d3.object());
}

                        // --------------------
                        // class bcefi_Vfunc2F6
                        // --------------------

template <class F, class A1, class A2, class D1, class D2, class D3, class D4>
inline
bcefi_Vfunc2F6<F, A1, A2, D1, D2, D3, D4>::bcefi_Vfunc2F6(
                                             F                procedure,
                                             const D1&        embeddedArg1,
                                             const D2&        embeddedArg2,
                                             const D3&        embeddedArg3,
                                             const D4&        embeddedArg4,
                                             bslma_Allocator *basicAllocator)
: bcefr_Vfunc2<A1, A2>(basicAllocator)
, d_f(procedure, basicAllocator)
, d_d1(embeddedArg1, basicAllocator)
, d_d2(embeddedArg2, basicAllocator)
, d_d3(embeddedArg3, basicAllocator)
, d_d4(embeddedArg4, basicAllocator)
{
}

template <class F, class A1, class A2, class D1, class D2, class D3, class D4>
inline
bcefi_Vfunc2F6<F, A1, A2, D1, D2, D3, D4>::~bcefi_Vfunc2F6()
{
}

template <class F, class A1, class A2, class D1, class D2, class D3, class D4>
inline
void bcefi_Vfunc2F6<F, A1, A2, D1, D2, D3, D4>::execute(
                                                   const A1& argument1,
                                                   const A2& argument2) const
{
    const F& f = d_f.object();  // for readability
    f(argument1, argument2, d_d1.object(), d_d2.object(), d_d3.object(),
                            d_d4.object());
}

                        // --------------------
                        // class bcefi_Vfunc2F7
                        // --------------------

template <class F, class A1, class A2, class D1, class D2, class D3, class D4,
          class D5>
inline
bcefi_Vfunc2F7<F, A1, A2, D1, D2, D3, D4, D5>::bcefi_Vfunc2F7(
                                             F                procedure,
                                             const D1&        embeddedArg1,
                                             const D2&        embeddedArg2,
                                             const D3&        embeddedArg3,
                                             const D4&        embeddedArg4,
                                             const D5&        embeddedArg5,
                                             bslma_Allocator *basicAllocator)
: bcefr_Vfunc2<A1, A2>(basicAllocator)
, d_f(procedure, basicAllocator)
, d_d1(embeddedArg1, basicAllocator)
, d_d2(embeddedArg2, basicAllocator)
, d_d3(embeddedArg3, basicAllocator)
, d_d4(embeddedArg4, basicAllocator)
, d_d5(embeddedArg5, basicAllocator)
{
}

template <class F, class A1, class A2, class D1, class D2, class D3, class D4,
          class D5>
inline
bcefi_Vfunc2F7<F, A1, A2, D1, D2, D3, D4, D5>::~bcefi_Vfunc2F7()
{
}

template <class F, class A1, class A2, class D1, class D2, class D3,
          class D4, class D5>
inline
void bcefi_Vfunc2F7<F, A1, A2, D1, D2, D3, D4, D5>::execute(
                                                   const A1& argument1,
                                                   const A2& argument2) const
{
    const F& f = d_f.object();  // for readability
    f(argument1, argument2, d_d1.object(), d_d2.object(), d_d3.object(),
                            d_d4.object(), d_d5.object());
}

                        // --------------------
                        // class bcefi_Vfunc2F8
                        // --------------------

template <class F, class A1, class A2, class D1, class D2, class D3, class D4,
          class D5, class D6>
inline
bcefi_Vfunc2F8<F, A1, A2, D1, D2, D3, D4, D5, D6>::bcefi_Vfunc2F8(
                                             F                procedure,
                                             const D1&        embeddedArg1,
                                             const D2&        embeddedArg2,
                                             const D3&        embeddedArg3,
                                             const D4&        embeddedArg4,
                                             const D5&        embeddedArg5,
                                             const D6&        embeddedArg6,
                                             bslma_Allocator *basicAllocator)
: bcefr_Vfunc2<A1, A2>(basicAllocator)
, d_f(procedure, basicAllocator)
, d_d1(embeddedArg1, basicAllocator)
, d_d2(embeddedArg2, basicAllocator)
, d_d3(embeddedArg3, basicAllocator)
, d_d4(embeddedArg4, basicAllocator)
, d_d5(embeddedArg5, basicAllocator)
, d_d6(embeddedArg6, basicAllocator)
{
}

template <class F, class A1, class A2, class D1, class D2, class D3, class D4,
          class D5, class D6>
inline
bcefi_Vfunc2F8<F, A1, A2, D1, D2, D3, D4, D5, D6>::~bcefi_Vfunc2F8()
{
}

template <class F, class A1, class A2, class D1, class D2, class D3,
          class D4, class D5, class D6>
inline
void bcefi_Vfunc2F8<F, A1, A2, D1, D2, D3, D4, D5, D6>::execute(
                                                   const A1& argument1,
                                                   const A2& argument2) const
{
    const F& f = d_f.object();  // for readability
    f(argument1, argument2, d_d1.object(), d_d2.object(), d_d3.object(),
                            d_d4.object(), d_d5.object(), d_d6.object());
}

                        // --------------------
                        // class bcefi_Vfunc2F9
                        // --------------------

template <class F, class A1, class A2, class D1, class D2, class D3, class D4,
          class D5, class D6, class D7>
inline
bcefi_Vfunc2F9<F, A1, A2, D1, D2, D3, D4, D5, D6, D7>::bcefi_Vfunc2F9(
                                             F                procedure,
                                             const D1&        embeddedArg1,
                                             const D2&        embeddedArg2,
                                             const D3&        embeddedArg3,
                                             const D4&        embeddedArg4,
                                             const D5&        embeddedArg5,
                                             const D6&        embeddedArg6,
                                             const D7&        embeddedArg7,
                                             bslma_Allocator *basicAllocator)
: bcefr_Vfunc2<A1, A2>(basicAllocator)
, d_f(procedure, basicAllocator)
, d_d1(embeddedArg1, basicAllocator)
, d_d2(embeddedArg2, basicAllocator)
, d_d3(embeddedArg3, basicAllocator)
, d_d4(embeddedArg4, basicAllocator)
, d_d5(embeddedArg5, basicAllocator)
, d_d6(embeddedArg6, basicAllocator)
, d_d7(embeddedArg7, basicAllocator)
{
}

template <class F, class A1, class A2, class D1, class D2, class D3, class D4,
          class D5, class D6, class D7>
inline
bcefi_Vfunc2F9<F, A1, A2, D1, D2, D3, D4, D5, D6, D7>::~bcefi_Vfunc2F9()
{
}

template <class F, class A1, class A2, class D1, class D2, class D3,
          class D4, class D5, class D6, class D7>
inline
void bcefi_Vfunc2F9<F, A1, A2, D1, D2, D3, D4, D5, D6, D7>::execute(
                                                   const A1& argument1,
                                                   const A2& argument2) const
{
    const F& f = d_f.object();  // for readability
    f(argument1, argument2, d_d1.object(), d_d2.object(), d_d3.object(),
                            d_d4.object(), d_d5.object(), d_d6.object(),
                            d_d7.object());
}

                        // --------------------
                        // class bcefi_Vfunc2C2
                        // --------------------

template <class OBJ, class C, class A1, class A2>
inline
bcefi_Vfunc2C2<OBJ, C, A1, A2>::bcefi_Vfunc2C2(const OBJ&       object,
                                               C                method,
                                               bslma_Allocator *basicAllocator)
: bcefr_Vfunc2<A1, A2>(basicAllocator)
, d_obj(object, basicAllocator)
, d_m(method)
{
}

template <class OBJ, class C, class A1, class A2>
inline
bcefi_Vfunc2C2<OBJ, C, A1, A2>::~bcefi_Vfunc2C2()
{
}

template <class OBJ, class C, class A1, class A2>
inline
void bcefi_Vfunc2C2<OBJ, C, A1, A2>::execute(const A1& argument1,
                                             const A2& argument2) const
{
    const OBJ& obj = d_obj.object();  // for readability
    (obj.*d_m)(argument1, argument2);
}

                        // --------------------
                        // class bcefi_Vfunc2C3
                        // --------------------

template <class OBJ, class C, class A1, class A2, class D1>
inline
bcefi_Vfunc2C3<OBJ, C, A1, A2, D1>::bcefi_Vfunc2C3(
                                             const OBJ&       object,
                                             C                method,
                                             const D1&        embeddedArg1,
                                             bslma_Allocator *basicAllocator)
: bcefr_Vfunc2<A1, A2>(basicAllocator)
, d_obj(object, basicAllocator)
, d_m(method)
, d_d1(embeddedArg1, basicAllocator)
{
}

template <class OBJ, class C, class A1, class A2, class D1>
inline
bcefi_Vfunc2C3<OBJ, C, A1, A2, D1>::~bcefi_Vfunc2C3()
{
}

template <class OBJ, class C, class A1, class A2, class D1>
inline
void bcefi_Vfunc2C3<OBJ, C, A1, A2, D1>::execute(const A1& argument1,
                                                 const A2& argument2) const
{
    const OBJ& obj = d_obj.object();  // for readability
    (obj.*d_m)(argument1, argument2, d_d1.object());
}

                        // --------------------
                        // class bcefi_Vfunc2C4
                        // --------------------

template <class OBJ, class C, class A1, class A2, class D1, class D2>
inline
bcefi_Vfunc2C4<OBJ, C, A1, A2, D1, D2>::bcefi_Vfunc2C4(
                                             const OBJ&       object,
                                             C                method,
                                             const D1&        embeddedArg1,
                                             const D2&        embeddedArg2,
                                             bslma_Allocator *basicAllocator)
: bcefr_Vfunc2<A1, A2>(basicAllocator)
, d_obj(object, basicAllocator)
, d_m(method)
, d_d1(embeddedArg1, basicAllocator)
, d_d2(embeddedArg2, basicAllocator)
{
}

template <class OBJ, class C, class A1, class A2, class D1, class D2>
inline
bcefi_Vfunc2C4<OBJ, C, A1, A2, D1, D2>::~bcefi_Vfunc2C4()
{
}

template <class OBJ, class C, class A1, class A2, class D1, class D2>
inline
void bcefi_Vfunc2C4<OBJ, C, A1, A2, D1, D2>::execute(
                                                   const A1& argument1,
                                                   const A2& argument2) const
{
    const OBJ& obj = d_obj.object();  // for readability
    (obj.*d_m)(argument1, argument2, d_d1.object(), d_d2.object());
}

                        // --------------------
                        // class bcefi_Vfunc2C5
                        // --------------------

template <class OBJ, class C, class A1, class A2, class D1, class D2, class D3>
inline
bcefi_Vfunc2C5<OBJ, C, A1, A2, D1, D2, D3>::bcefi_Vfunc2C5(
                                             const OBJ&       object,
                                             C                method,
                                             const D1&        embeddedArg1,
                                             const D2&        embeddedArg2,
                                             const D3&        embeddedArg3,
                                             bslma_Allocator *basicAllocator)
: bcefr_Vfunc2<A1, A2>(basicAllocator)
, d_obj(object, basicAllocator)
, d_m(method)
, d_d1(embeddedArg1, basicAllocator)
, d_d2(embeddedArg2, basicAllocator)
, d_d3(embeddedArg3, basicAllocator)
{
}

template <class OBJ, class C, class A1, class A2, class D1, class D2, class D3>
inline
bcefi_Vfunc2C5<OBJ, C, A1, A2, D1, D2, D3>::~bcefi_Vfunc2C5()
{
}

template <class OBJ, class C, class A1, class A2, class D1, class D2, class D3>
inline
void bcefi_Vfunc2C5<OBJ, C, A1, A2, D1, D2, D3>::execute(
                                                   const A1& argument1,
                                                   const A2& argument2) const
{
    const OBJ& obj = d_obj.object();  // for readability
    (obj.*d_m)(argument1, argument2, d_d1.object(), d_d2.object(),
                                     d_d3.object());
}

                        // --------------------
                        // class bcefi_Vfunc2C6
                        // --------------------

template <class OBJ, class C, class A1, class A2, class D1, class D2, class D3,
          class D4>
inline
bcefi_Vfunc2C6<OBJ, C, A1, A2, D1, D2, D3, D4>::bcefi_Vfunc2C6(
                                             const OBJ&       object,
                                             C                method,
                                             const D1&        embeddedArg1,
                                             const D2&        embeddedArg2,
                                             const D3&        embeddedArg3,
                                             const D4&        embeddedArg4,
                                             bslma_Allocator *basicAllocator)
: bcefr_Vfunc2<A1, A2>(basicAllocator)
, d_obj(object, basicAllocator)
, d_m(method)
, d_d1(embeddedArg1, basicAllocator)
, d_d2(embeddedArg2, basicAllocator)
, d_d3(embeddedArg3, basicAllocator)
, d_d4(embeddedArg4, basicAllocator)
{
}

template <class OBJ, class C, class A1, class A2, class D1, class D2, class D3,
          class D4>
inline
bcefi_Vfunc2C6<OBJ, C, A1, A2, D1, D2, D3, D4>::~bcefi_Vfunc2C6()
{
}

template <class OBJ, class C, class A1, class A2, class D1, class D2, class D3,
          class D4>
inline
void bcefi_Vfunc2C6<OBJ, C, A1, A2, D1, D2, D3, D4>::execute(
                                                   const A1& argument1,
                                                   const A2& argument2) const
{
    const OBJ& obj = d_obj.object();  // for readability
    (obj.*d_m)(argument1, argument2, d_d1.object(), d_d2.object(),
                                     d_d3.object(), d_d4.object());
}

                        // --------------------
                        // class bcefi_Vfunc2C7
                        // --------------------

template <class OBJ, class C, class A1, class A2, class D1, class D2, class D3,
          class D4, class D5>
inline
bcefi_Vfunc2C7<OBJ, C, A1, A2, D1, D2, D3, D4, D5>::bcefi_Vfunc2C7(
                                             const OBJ&       object,
                                             C                method,
                                             const D1&        embeddedArg1,
                                             const D2&        embeddedArg2,
                                             const D3&        embeddedArg3,
                                             const D4&        embeddedArg4,
                                             const D5&        embeddedArg5,
                                             bslma_Allocator *basicAllocator)
: bcefr_Vfunc2<A1, A2>(basicAllocator)
, d_obj(object, basicAllocator)
, d_m(method)
, d_d1(embeddedArg1, basicAllocator)
, d_d2(embeddedArg2, basicAllocator)
, d_d3(embeddedArg3, basicAllocator)
, d_d4(embeddedArg4, basicAllocator)
, d_d5(embeddedArg5, basicAllocator)
{
}

template <class OBJ, class C, class A1, class A2, class D1, class D2, class D3,
          class D4, class D5>
inline
bcefi_Vfunc2C7<OBJ, C, A1, A2, D1, D2, D3, D4, D5>::~bcefi_Vfunc2C7()
{
}

template <class OBJ, class C, class A1, class A2, class D1, class D2, class D3,
          class D4, class D5>
inline
void bcefi_Vfunc2C7<OBJ, C, A1, A2, D1, D2, D3, D4, D5>::execute(
                                                   const A1& argument1,
                                                   const A2& argument2) const
{
    const OBJ& obj = d_obj.object();  // for readability
    (obj.*d_m)(argument1, argument2, d_d1.object(), d_d2.object(),
                                     d_d3.object(), d_d4.object(),
                                     d_d5.object());
}

                        // --------------------
                        // class bcefi_Vfunc2C8
                        // --------------------

template <class OBJ, class C, class A1, class A2, class D1, class D2, class D3,
          class D4, class D5, class D6>
inline
bcefi_Vfunc2C8<OBJ, C, A1, A2, D1, D2, D3, D4, D5, D6>::bcefi_Vfunc2C8(
                                             const OBJ&       object,
                                             C                method,
                                             const D1&        embeddedArg1,
                                             const D2&        embeddedArg2,
                                             const D3&        embeddedArg3,
                                             const D4&        embeddedArg4,
                                             const D5&        embeddedArg5,
                                             const D6&        embeddedArg6,
                                             bslma_Allocator *basicAllocator)
: bcefr_Vfunc2<A1, A2>(basicAllocator)
, d_obj(object, basicAllocator)
, d_m(method)
, d_d1(embeddedArg1, basicAllocator)
, d_d2(embeddedArg2, basicAllocator)
, d_d3(embeddedArg3, basicAllocator)
, d_d4(embeddedArg4, basicAllocator)
, d_d5(embeddedArg5, basicAllocator)
, d_d6(embeddedArg6, basicAllocator)
{
}

template <class OBJ, class C, class A1, class A2, class D1, class D2, class D3,
          class D4, class D5, class D6>
inline
bcefi_Vfunc2C8<OBJ, C, A1, A2, D1, D2, D3, D4, D5, D6>::~bcefi_Vfunc2C8()
{
}

template <class OBJ, class C, class A1, class A2, class D1, class D2, class D3,
          class D4, class D5, class D6>
inline
void bcefi_Vfunc2C8<OBJ, C, A1, A2, D1, D2, D3, D4, D5, D6>::execute(
                                                   const A1& argument1,
                                                   const A2& argument2) const
{
    const OBJ& obj = d_obj.object();  // for readability
    (obj.*d_m)(argument1, argument2, d_d1.object(), d_d2.object(),
                                     d_d3.object(), d_d4.object(),
                                     d_d5.object(), d_d6.object());
}

                        // --------------------
                        // class bcefi_Vfunc2C9
                        // --------------------

template <class OBJ, class C, class A1, class A2, class D1, class D2, class D3,
          class D4, class D5, class D6, class D7>
inline
bcefi_Vfunc2C9<OBJ, C, A1, A2, D1, D2, D3, D4, D5, D6, D7>::bcefi_Vfunc2C9(
                                             const OBJ&       object,
                                             C                method,
                                             const D1&        embeddedArg1,
                                             const D2&        embeddedArg2,
                                             const D3&        embeddedArg3,
                                             const D4&        embeddedArg4,
                                             const D5&        embeddedArg5,
                                             const D6&        embeddedArg6,
                                             const D7&        embeddedArg7,
                                             bslma_Allocator *basicAllocator)
: bcefr_Vfunc2<A1, A2>(basicAllocator)
, d_obj(object, basicAllocator)
, d_m(method)
, d_d1(embeddedArg1, basicAllocator)
, d_d2(embeddedArg2, basicAllocator)
, d_d3(embeddedArg3, basicAllocator)
, d_d4(embeddedArg4, basicAllocator)
, d_d5(embeddedArg5, basicAllocator)
, d_d6(embeddedArg6, basicAllocator)
, d_d7(embeddedArg7, basicAllocator)
{
}

template <class OBJ, class C, class A1, class A2, class D1, class D2, class D3,
          class D4, class D5, class D6, class D7>
inline
bcefi_Vfunc2C9<OBJ, C, A1, A2, D1, D2, D3, D4, D5, D6, D7>::~bcefi_Vfunc2C9()
{
}

template <class OBJ, class C, class A1, class A2, class D1, class D2, class D3,
          class D4, class D5, class D6, class D7>
inline
void bcefi_Vfunc2C9<OBJ, C, A1, A2, D1, D2, D3, D4, D5, D6, D7>::execute(
                                                   const A1& argument1,
                                                   const A2& argument2) const
{
    const OBJ& obj = d_obj.object();  // for readability
    (obj.*d_m)(argument1, argument2, d_d1.object(), d_d2.object(),
                                     d_d3.object(), d_d4.object(),
                                     d_d5.object(), d_d6.object(),
                                     d_d7.object());
}

                        // --------------------
                        // class bcefi_Vfunc2M2
                        // --------------------

template <class OBJ, class M, class A1, class A2>
inline
bcefi_Vfunc2M2<OBJ, M, A1, A2>::bcefi_Vfunc2M2(OBJ             *object,
                                               M                method,
                                               bslma_Allocator *basicAllocator)
: bcefr_Vfunc2<A1, A2>(basicAllocator)
, d_obj_p(object)
, d_m(method)
{
}

template <class OBJ, class M, class A1, class A2>
inline
bcefi_Vfunc2M2<OBJ, M, A1, A2>::~bcefi_Vfunc2M2()
{
}

template <class OBJ, class M, class A1, class A2>
inline
void bcefi_Vfunc2M2<OBJ, M, A1, A2>::execute(const A1& argument1,
                                             const A2& argument2) const
{
    (d_obj_p->*d_m)(argument1, argument2);
}

                        // --------------------
                        // class bcefi_Vfunc2M3
                        // --------------------

template <class OBJ, class M, class A1, class A2, class D1>
inline
bcefi_Vfunc2M3<OBJ, M, A1, A2, D1>::bcefi_Vfunc2M3(
                                             OBJ             *object,
                                             M                method,
                                             const D1&        embeddedArg1,
                                             bslma_Allocator *basicAllocator)
: bcefr_Vfunc2<A1, A2>(basicAllocator)
, d_obj_p(object)
, d_m(method)
, d_d1(embeddedArg1, basicAllocator)
{
}

template <class OBJ, class M, class A1, class A2, class D1>
inline
bcefi_Vfunc2M3<OBJ, M, A1, A2, D1>::~bcefi_Vfunc2M3()
{
}

template <class OBJ, class M, class A1, class A2, class D1>
inline
void bcefi_Vfunc2M3<OBJ, M, A1, A2, D1>::execute(const A1& argument1,
                                                 const A2& argument2) const
{
    (d_obj_p->*d_m)(argument1, argument2, d_d1.object());
}

                        // --------------------
                        // class bcefi_Vfunc2M4
                        // --------------------

template <class OBJ, class M, class A1, class A2, class D1, class D2>
inline
bcefi_Vfunc2M4<OBJ, M, A1, A2, D1, D2>::bcefi_Vfunc2M4(
                                             OBJ             *object,
                                             M                method,
                                             const D1&        embeddedArg1,
                                             const D2&        embeddedArg2,
                                             bslma_Allocator *basicAllocator)
: bcefr_Vfunc2<A1, A2>(basicAllocator)
, d_obj_p(object)
, d_m(method)
, d_d1(embeddedArg1, basicAllocator)
, d_d2(embeddedArg2, basicAllocator)
{
}

template <class OBJ, class M, class A1, class A2, class D1, class D2>
inline
bcefi_Vfunc2M4<OBJ, M, A1, A2, D1, D2>::~bcefi_Vfunc2M4()
{
}

template <class OBJ, class M, class A1, class A2, class D1, class D2>
inline
void bcefi_Vfunc2M4<OBJ, M, A1, A2, D1, D2>::execute(
                                                   const A1& argument1,
                                                   const A2& argument2) const
{
    (d_obj_p->*d_m)(argument1, argument2, d_d1.object(), d_d2.object());
}

                        // --------------------
                        // class bcefi_Vfunc2M5
                        // --------------------

template <class OBJ, class M, class A1, class A2, class D1, class D2, class D3>
inline
bcefi_Vfunc2M5<OBJ, M, A1, A2, D1, D2, D3>::bcefi_Vfunc2M5(
                                             OBJ             *object,
                                             M                method,
                                             const D1&        embeddedArg1,
                                             const D2&        embeddedArg2,
                                             const D3&        embeddedArg3,
                                             bslma_Allocator *basicAllocator)
: bcefr_Vfunc2<A1, A2>(basicAllocator)
, d_obj_p(object)
, d_m(method)
, d_d1(embeddedArg1, basicAllocator)
, d_d2(embeddedArg2, basicAllocator)
, d_d3(embeddedArg3, basicAllocator)
{
}

template <class OBJ, class M, class A1, class A2, class D1, class D2, class D3>
inline
bcefi_Vfunc2M5<OBJ, M, A1, A2, D1, D2, D3>::~bcefi_Vfunc2M5()
{
}

template <class OBJ, class M, class A1, class A2, class D1, class D2, class D3>
inline
void bcefi_Vfunc2M5<OBJ, M, A1, A2, D1, D2, D3>::execute(
                                                     const A1& argument1,
                                                     const A2& argument2) const
{
    (d_obj_p->*d_m)(argument1, argument2, d_d1.object(), d_d2.object(),
                                          d_d3.object());
}

                        // --------------------
                        // class bcefi_Vfunc2M6
                        // --------------------

template <class OBJ, class M, class A1, class A2, class D1, class D2, class D3,
          class D4>
inline
bcefi_Vfunc2M6<OBJ, M, A1, A2, D1, D2, D3, D4>::bcefi_Vfunc2M6(
                                             OBJ             *object,
                                             M                method,
                                             const D1&        embeddedArg1,
                                             const D2&        embeddedArg2,
                                             const D3&        embeddedArg3,
                                             const D4&        embeddedArg4,
                                             bslma_Allocator *basicAllocator)
: bcefr_Vfunc2<A1, A2>(basicAllocator)
, d_obj_p(object)
, d_m(method)
, d_d1(embeddedArg1, basicAllocator)
, d_d2(embeddedArg2, basicAllocator)
, d_d3(embeddedArg3, basicAllocator)
, d_d4(embeddedArg4, basicAllocator)
{
}

template <class OBJ, class M, class A1, class A2, class D1, class D2, class D3,
          class D4>
inline
bcefi_Vfunc2M6<OBJ, M, A1, A2, D1, D2, D3, D4>::~bcefi_Vfunc2M6()
{
}

template <class OBJ, class M, class A1, class A2, class D1, class D2, class D3,
          class D4>
inline
void bcefi_Vfunc2M6<OBJ, M, A1, A2, D1, D2, D3, D4>::execute(
                                                   const A1& argument1,
                                                   const A2& argument2) const
{
    (d_obj_p->*d_m)(argument1, argument2, d_d1.object(), d_d2.object(),
                                          d_d3.object(), d_d4.object());
}

                        // --------------------
                        // class bcefi_Vfunc2M7
                        // --------------------

template <class OBJ, class M, class A1, class A2, class D1, class D2, class D3,
          class D4, class D5>
inline
bcefi_Vfunc2M7<OBJ, M, A1, A2, D1, D2, D3, D4, D5>::bcefi_Vfunc2M7(
                                             OBJ             *object,
                                             M                method,
                                             const D1&        embeddedArg1,
                                             const D2&        embeddedArg2,
                                             const D3&        embeddedArg3,
                                             const D4&        embeddedArg4,
                                             const D5&        embeddedArg5,
                                             bslma_Allocator *basicAllocator)
: bcefr_Vfunc2<A1, A2>(basicAllocator)
, d_obj_p(object)
, d_m(method)
, d_d1(embeddedArg1, basicAllocator)
, d_d2(embeddedArg2, basicAllocator)
, d_d3(embeddedArg3, basicAllocator)
, d_d4(embeddedArg4, basicAllocator)
, d_d5(embeddedArg5, basicAllocator)
{
}

template <class OBJ, class M, class A1, class A2, class D1, class D2, class D3,
          class D4, class D5>
inline
bcefi_Vfunc2M7<OBJ, M, A1, A2, D1, D2, D3, D4, D5>::~bcefi_Vfunc2M7()
{
}

template <class OBJ, class M, class A1, class A2, class D1, class D2, class D3,
          class D4, class D5>
inline
void bcefi_Vfunc2M7<OBJ, M, A1, A2, D1, D2, D3, D4, D5>::execute(
                                                   const A1& argument1,
                                                   const A2& argument2) const
{
    (d_obj_p->*d_m)(argument1, argument2, d_d1.object(), d_d2.object(),
                                          d_d3.object(), d_d4.object(),
                                          d_d5.object());
}

                        // --------------------
                        // class bcefi_Vfunc2M8
                        // --------------------

template <class OBJ, class M, class A1, class A2, class D1, class D2, class D3,
          class D4, class D5, class D6>
inline
bcefi_Vfunc2M8<OBJ, M, A1, A2, D1, D2, D3, D4, D5, D6>::bcefi_Vfunc2M8(
                                             OBJ             *object,
                                             M                method,
                                             const D1&        embeddedArg1,
                                             const D2&        embeddedArg2,
                                             const D3&        embeddedArg3,
                                             const D4&        embeddedArg4,
                                             const D5&        embeddedArg5,
                                             const D6&        embeddedArg6,
                                             bslma_Allocator *basicAllocator)
: bcefr_Vfunc2<A1, A2>(basicAllocator)
, d_obj_p(object)
, d_m(method)
, d_d1(embeddedArg1, basicAllocator)
, d_d2(embeddedArg2, basicAllocator)
, d_d3(embeddedArg3, basicAllocator)
, d_d4(embeddedArg4, basicAllocator)
, d_d5(embeddedArg5, basicAllocator)
, d_d6(embeddedArg6, basicAllocator)
{
}

template <class OBJ, class M, class A1, class A2, class D1, class D2, class D3,
          class D4, class D5, class D6>
inline
bcefi_Vfunc2M8<OBJ, M, A1, A2, D1, D2, D3, D4, D5, D6>::~bcefi_Vfunc2M8()
{
}

template <class OBJ, class M, class A1, class A2, class D1, class D2, class D3,
          class D4, class D5, class D6>
inline
void bcefi_Vfunc2M8<OBJ, M, A1, A2, D1, D2, D3, D4, D5, D6>::execute(
                                                   const A1& argument1,
                                                   const A2& argument2) const
{
    (d_obj_p->*d_m)(argument1, argument2, d_d1.object(), d_d2.object(),
                                          d_d3.object(), d_d4.object(),
                                          d_d5.object(), d_d6.object());
}

                        // --------------------
                        // class bcefi_Vfunc2M9
                        // --------------------

template <class OBJ, class M, class A1, class A2, class D1, class D2, class D3,
          class D4, class D5, class D6, class D7>
inline
bcefi_Vfunc2M9<OBJ, M, A1, A2, D1, D2, D3, D4, D5, D6, D7>::bcefi_Vfunc2M9(
                                             OBJ             *object,
                                             M                method,
                                             const D1&        embeddedArg1,
                                             const D2&        embeddedArg2,
                                             const D3&        embeddedArg3,
                                             const D4&        embeddedArg4,
                                             const D5&        embeddedArg5,
                                             const D6&        embeddedArg6,
                                             const D7&        embeddedArg7,
                                             bslma_Allocator *basicAllocator)
: bcefr_Vfunc2<A1, A2>(basicAllocator)
, d_obj_p(object)
, d_m(method)
, d_d1(embeddedArg1, basicAllocator)
, d_d2(embeddedArg2, basicAllocator)
, d_d3(embeddedArg3, basicAllocator)
, d_d4(embeddedArg4, basicAllocator)
, d_d5(embeddedArg5, basicAllocator)
, d_d6(embeddedArg6, basicAllocator)
, d_d7(embeddedArg7, basicAllocator)
{
}

template <class OBJ, class M, class A1, class A2, class D1, class D2, class D3,
          class D4, class D5, class D6, class D7>
inline
bcefi_Vfunc2M9<OBJ, M, A1, A2, D1, D2, D3, D4, D5, D6, D7>::~bcefi_Vfunc2M9()
{
}

template <class OBJ, class M, class A1, class A2, class D1, class D2, class D3,
          class D4, class D5, class D6, class D7>
inline
void bcefi_Vfunc2M9<OBJ, M, A1, A2, D1, D2, D3, D4, D5, D6, D7>::execute(
                                                   const A1& argument1,
                                                   const A2& argument2) const
{
    (d_obj_p->*d_m)(argument1, argument2, d_d1.object(), d_d2.object(),
                                          d_d3.object(), d_d4.object(),
                                          d_d5.object(), d_d6.object(),
                                          d_d7.object());
}

}  // close namespace BloombergLP

#endif

// ---------------------------------------------------------------------------
// NOTICE:
//      Copyright (C) Bloomberg L.P., 2002
//      All Rights Reserved.
//      Property of Bloomberg L.P. (BLP)
//      This software is made available solely pursuant to the
//      terms of a BLP license agreement which governs its use.
// ----------------------------- END-OF-FILE ---------------------------------
