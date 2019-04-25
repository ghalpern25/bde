// bslmf_istriviallycopyable.h                                        -*-C++-*-
#ifndef INCLUDED_BSLMF_ISTRIVIALLYCOPYABLE
#define INCLUDED_BSLMF_ISTRIVIALLYCOPYABLE

#include <bsls_ident.h>
BSLS_IDENT("$Id: $")

//@PURPOSE: Provide a meta-function for determining trivially copyable types.
//
//@CLASSES:
//  bsl::is_trivially_copyable: type-traits meta-function
//  bsl::is_trivially_copyable_v: the result value of the meta-function
//
//@SEE_ALSO: bslmf_integralconstant, bslmf_nestedtraitdeclaration
//
//@DESCRIPTION: This component defines a meta-function,
// 'bsl::is_trivially_copyable' and a template variable
// 'bsl::is_trivially_copyable_v', that represents the result value of the
// 'bsl::is_trivially_copyable' meta-function, that may be used to query
// whether a type is trivially copyable as defined in section 3.9.3 of the
// C++11 standard [basic.types].
//
// 'bsl::is_trivially_copyable' has the same syntax as the
// 'is_trivially_copyable' template from the C++11 standard [meta.unary.prop].
// However, unlike the template defined in the C++11 standard, which can
// determine the correct value for all types without requiring specialization,
// 'bsl::is_trivially_copyable' can, by default, determine the value for the
// following type categories only:
//..
//  Type Category        Is Trivially Copyable
//  -------------        ---------------------
//  reference types      false
//  fundamental types    true
//  enumerated types     true
//  pointers             true
//  pointers to members  true
//..
// For all other types, 'bsl::is_trivially_copyable' returns 'false', unless
// the type is explicitly specified to be trivially copyable.  This can be done
// in 2 ways:
//
//: 1 Define a template specialization for 'bsl::is_trivially_copyable' having
//:   the type as the template parameter that inherits directly from
//:   'bsl::true_type'.
//:
//: 2 Use the 'BSLMF_NESTED_TRAIT_DECLARATION' macro to define
//:   'bsl::is_trivially_copyable' as a trait in the class definition of the
//:   type.
//
// Note that the template variable 'is_trivially_copyable_v' is defined in the
// C++17 standard as an inline variable.  If the current compiler supports the
// inline variable C++17 compiler feature, 'bsl::is_trivially_copyable_v' is
// defined as an 'inline constexpr bool' variable.  Otherwise, if the compiler
// supports the variable templates C++14 compiler feature,
// 'bsl::is_trivially_copyable_v' is defined as a non-inline 'constexpr bool'
// variable.  See 'BSLS_COMPILERFEATURES_SUPPORT_INLINE_VARIABLES' and
// 'BSLS_COMPILERFEATURES_SUPPORT_VARIABLE_TEMPLATES' macros in
// bsls_compilerfeatures component for details.
//
///Usage
///-----
// In this section we show intended use of this component.
//
///Example 1: Verify Whether Types are Trivially Copyable
/// - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Suppose that we want to assert whether a type is trivially copyable.
//
// First, we define a set of types to evaluate:
//..
//  typedef int  MyFundamentalType;
//  typedef int& MyFundamentalTypeReference;
//
//  class MyTriviallyCopyableType {
//  };
//
//  struct MyNonTriviallyCopyableType {
//      //...
//  };
//..
// Then, since user-defined types cannot be automatically evaluated by
// 'is_trivially_copyable', we define a template specialization to specify that
// 'MyTriviallyCopyableType' is trivially copyable:
//..
//  namespace bsl {
//
//  template <>
//  struct is_trivially_copyable<MyTriviallyCopyableType> : bsl::true_type {
//      // This template specialization for 'is_trivially_copyable' indicates
//      // that 'MyTriviallyCopyableType' is a trivially copyable type.
//  };
//
//  }  // close namespace bsl
//..
// Now, we verify whether each type is trivially copyable using
// 'bsl::is_trivially_copyable':
//..
//  assert(true  == bsl::is_trivially_copyable<MyFundamentalType>::value);
//  assert(false == bsl::is_trivially_copyable<
//                                         MyFundamentalTypeReference>::value);
//  assert(true  == bsl::is_trivially_copyable<
//                                            MyTriviallyCopyableType>::value);
//  assert(false == bsl::is_trivially_copyable<
//                                         MyNonTriviallyCopyableType>::value);
//..
// Note that if the current compiler supports the variable templates C++14
// feature, then we can re-write the snippet of code above as follows:
//..
//  #ifdef BSLS_COMPILERFEATURES_SUPPORT_VARIABLE_TEMPLATES
//  assert(true  == bsl::is_trivially_copyable_v<MyFundamentalType>);
//  assert(false == bsl::is_trivially_copyable_v<MyFundamentalTypeReference>);
//  assert(true  == bsl::is_trivially_copyable_v<MyTriviallyCopyableType>);
//  assert(false == bsl::is_trivially_copyable_v<MyNonTriviallyCopyableType>);
//  #endif
//..

#include <bslscm_version.h>

#include <bslmf_detectnestedtrait.h>
#include <bslmf_integralconstant.h>
#include <bslmf_isenum.h>
#include <bslmf_isfundamental.h>
#include <bslmf_ismemberpointer.h>
#include <bslmf_ispointer.h>

#include <bsls_compilerfeatures.h>
#include <bsls_keyword.h>
#include <bsls_platform.h>
#include <bsls_timeinterval.h>

#include <stddef.h>

#ifndef BDE_DONT_ALLOW_TRANSITIVE_INCLUDES
#include <bslmf_ispointertomember.h>
#endif // BDE_DONT_ALLOW_TRANSITIVE_INCLUDES

// This set of includes must come last, due to usage of 'bsls_nativestd'
#ifdef BSLS_COMPILERFEATURES_SUPPORT_TRAITS_HEADER
# include <bsls_nativestd.h>

# define BSLMF_INCLUDE_ONLY_NATIVE_TRAITS
# include <type_traits>
#endif // BSLS_COMPILERFEATURES_SUPPORT_TRAITS_HEADER

#ifdef BSLS_COMPILERFEATURES_SUPPORT_TRAITS_HEADER
#define BSLMF_ISTRIVIALLYCOPYABLE_NATIVE_IMPLEMENTATION
// Early implementations of C++11 type traits did not always provide the
// necessary compiler intrinsic to detect the 'trivial' traits, so we use an
// additional component-level feature macro to detect whether native support is
// truly present.  This macro is defined for Visual C++ prior to VC2015 due to
// wrong results for certain types with the initial implementation of that
// trait.

#if (defined(BSLS_PLATFORM_CMP_GNU)  && BSLS_PLATFORM_CMP_VERSION < 50000)    \
 || (defined(BSLS_PLATFORM_CMP_MSVC) && BSLS_PLATFORM_CMP_VERSION < 1900)
# undef BSLMF_ISTRIVIALLYCOPYABLE_NATIVE_IMPLEMENTATION
#endif

#endif

#ifndef BSLMF_ISTRIVIALLYCOPYABLE_NATIVE_IMPLEMENTATION
#include <bslmf_nil.h>
#endif

namespace bsl {

template <class TYPE>
struct is_trivially_copyable;

}  // close namespace bsl

namespace BloombergLP {
namespace bslmf {

                         // ==============================
                         // struct IsTriviallyCopyable_Imp
                         // ==============================

template <class TYPE, bool K_INTRINSIC = false>
struct IsTriviallyCopyable_DetectTrait
    :  DetectNestedTrait<TYPE, bsl::is_trivially_copyable>::type {
    // This 'struct' template implements a meta-function to determine whether
    // the (non-cv-qualified) (template parameter) 'TYPE' has been explicitly
    // tagged with the trivially copyable trait.  If the flag 'K_INTRINSIC' is
    // 'true' then the compiler has already determined that 'TYPE' is trivially
    // copyable without user intervention, and the check for nested traits can
    // be optimized away.
};

template <class TYPE>
struct IsTriviallyCopyable_DetectTrait<TYPE, true>
    :  bsl::true_type {
    // This 'struct' template implements a meta-function to determine whether
    // the (non-cv-qualified) (template parameter) 'TYPE' is trivially
    // copyable.
};

#ifdef BSLMF_ISTRIVIALLYCOPYABLE_NATIVE_IMPLEMENTATION
template <class TYPE>
struct IsTriviallyCopyable_Intrinsic
    : IsTriviallyCopyable_DetectTrait<
                      TYPE,
                      ::native_std::is_trivially_copyable<TYPE>::value>::type {
    // This 'struct' template implements a meta-function to determine whether
    // the (non-cv-qualified) (template parameter) 'TYPE' is trivially
    // copyable.
};
#else
template <class TYPE>
struct IsTriviallyCopyable_Intrinsic
    : IsTriviallyCopyable_DetectTrait<
                                TYPE,
                                bsl::is_fundamental<TYPE>::value
                                || bsl::is_enum<TYPE>::value
                                || bsl::is_pointer<TYPE>::value
                                || bsl::is_member_pointer<TYPE>::value>::type {
    // This 'struct' template implements a meta-function to determine whether
    // the (non-cv-qualified) (template parameter) 'TYPE' is trivially
    // copyable.  Without compiler support, only scalar types are trivial
    // copyable.
};

template <>
struct IsTriviallyCopyable_Intrinsic<void> : bsl::false_type {
    // This explicit specialization reports that 'void' is not a trivially
    // copyable type, despite being a fundamental type.
};
#endif

}  // close package namespace
}  // close enterprise namespace

namespace bsl {

                         // ============================
                         // struct is_trivially_copyable
                         // ============================

template <class TYPE>
struct is_trivially_copyable
: BloombergLP::bslmf::IsTriviallyCopyable_Intrinsic<TYPE>::type {
    // This 'struct' template implements a meta-function to determine whether
    // the (template parameter) 'TYPE' is trivially copyable.  This 'struct'
    // derives from 'bsl::true_type' if the 'TYPE' is trivially copyable, and
    // from 'bsl::false_type' otherwise.  This meta-function has the same
    // syntax as the 'is_trivially_copyable' meta-function defined in the C++11
    // standard [meta.unary.prop]; however, this meta-function can
    // automatically determine the value for the following types only:
    // reference types, fundamental types, enumerated types, pointers to
    // members, and types declared to have the 'bsl::is_trivially_copyable'
    // trait using the 'BSLMF_NESTED_TRAIT_DECLARATION' macro (the value for
    // other types defaults to 'false').  To support other trivially copyable
    // types, this template must be specialized to inherit from
    // 'bsl::true_type' for them.
};

#ifdef BSLS_COMPILERFEATURES_SUPPORT_VARIABLE_TEMPLATES
template <class TYPE>
BSLS_KEYWORD_INLINE_VARIABLE
constexpr bool is_trivially_copyable_v = is_trivially_copyable<TYPE>::value;
    // This template variable represents the result value of the
    // 'bsl::is_trivially_copyable' meta-function.
#endif

///IMPLEMENTATION NOTE
///-------------------
// We specialize 'is_trivially_copyable' for 'bsls::TimeInterval' here because
// 'bsls' is levelized below 'bslmf'.  Previously, 'bsls_timeinterval.h' had
// forward declared the 'is_trivially_copyable' template and provided a
// specialization for 'TimeInterval' (see BDE 2.24.0 tag), but the forward
// declaration caused compilation errors with the Sun CC 5.13 compiler.
//
// We specialize 'is_trivially_copyable' for 'bslmf::Nil' here to avoid
// increasing the dependency envelope of 'bslmf_nil'.
//
// Neither of these trait declarations will be needed once we fully migrate to
// a C++11 definition for 'is_trivially_copyable'.

template <>
struct is_trivially_copyable<BloombergLP::bsls::TimeInterval> : bsl::true_type{
    // This template specialization for 'is_trivially_copyable' indicates that
    // 'TimeInterval' is a trivially copyable type.
};

#ifndef BSLMF_ISTRIVIALLYCOPYABLE_NATIVE_IMPLEMENTATION
template <>
struct is_trivially_copyable<BloombergLP::bslmf::Nil> : bsl::true_type {
    // This template specialization for 'is_trivially_copyable' indicates that
    // 'Nil' is a trivially copyable type.
};
#endif

template <class TYPE>
struct is_trivially_copyable<const TYPE>
    :  is_trivially_copyable<TYPE>::type {
    // This partial specialization ensures that const-qualified types have the
    // same result as their element type.
};

template <class TYPE>
struct is_trivially_copyable<volatile TYPE>
    :  is_trivially_copyable<TYPE>::type {
    // This partial specialization ensures that volatile-qualified types have
    // the same result as their element type.
};

template <class TYPE>
struct is_trivially_copyable<const volatile TYPE>
    :  is_trivially_copyable<TYPE>::type {
    // This partial specialization ensures that const-volatile-qualified types
    // have the same result as their element type.
};

template <class TYPE, size_t LEN>
struct is_trivially_copyable<TYPE[LEN]>
    :  is_trivially_copyable<TYPE>::type {
    // This partial specialization ensures that array types have the same
    // result as their element type.
};

template <class TYPE, size_t LEN>
struct is_trivially_copyable<const TYPE[LEN]>
    :  is_trivially_copyable<TYPE>::type {
    // This partial specialization ensures that const-qualified array types
    // have the same result as their element type.
};

template <class TYPE, size_t LEN>
struct is_trivially_copyable<volatile TYPE[LEN]>
    :  is_trivially_copyable<TYPE>::type {
    // This partial specialization ensures that volatile-qualified array types
    // have the same result as their element type.
};

template <class TYPE, size_t LEN>
struct is_trivially_copyable<const volatile TYPE[LEN]>
    :  is_trivially_copyable<TYPE>::type {
    // This partial specialization ensures that const-volatile-qualified array
    // types have the same result as their element type.
};

#if !defined(BSLS_PLATFORM_CMP_IBM)
// Last checked with the xlC 12.1 compiler.  The IBM xlC compiler has problems
// correctly handling arrays of unknown bound as template parameters.

template <class TYPE>
struct is_trivially_copyable<TYPE[]>
    :  is_trivially_copyable<TYPE>::type {
    // This partial specialization ensures that array-of-unknown-bound types
    // have the same result as their element type.
};

template <class TYPE>
struct is_trivially_copyable<const TYPE[]>
    :  is_trivially_copyable<TYPE>::type {
    // This partial specialization ensures that const-qualified
    // array-of-unknown-bound types have the same result as their element type.
};

template <class TYPE>
struct is_trivially_copyable<volatile TYPE[]>
    :  is_trivially_copyable<TYPE>::type {
    // This partial specialization ensures that volatile-qualified
    // array-of-unknown-bound types have the same result as their element type.
};

template <class TYPE>
struct is_trivially_copyable<const volatile TYPE[]>
    :  is_trivially_copyable<TYPE>::type {
    // This partial specialization ensures that const-volatile-qualified
    // array-of-unknown-bound types have the same result as their element type.
};
#endif

template <class TYPE>
struct is_trivially_copyable<TYPE &> :  false_type {
    // This partial specialization optimizes away a number of nested template
    // instantiations to prove that reference types are never trivially
    // copyable.
};

#if defined(BSLS_COMPILERFEATURES_SUPPORT_RVALUE_REFERENCES)
template <class TYPE>
struct is_trivially_copyable<TYPE &&> :  false_type {
    // This partial specialization optimizes away a number of nested template
    // instantiations to prove that reference types are never trivially
    // copyable.
};
#endif

}  // close namespace bsl

#endif // ! defined(INCLUDED_BSLMF_ISTRIVIALLYCOPYABLE)

// ----------------------------------------------------------------------------
// Copyright 2013 Bloomberg Finance L.P.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// ----------------------------- END-OF-FILE ----------------------------------
