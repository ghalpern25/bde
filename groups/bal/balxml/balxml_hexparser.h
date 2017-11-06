// balxml_hexparser.h                                                 -*-C++-*-

// ----------------------------------------------------------------------------
//                                   NOTICE
//
// This component is not up to date with current BDE coding standards, and
// should not be used as an example for new development.
// ----------------------------------------------------------------------------

#ifndef INCLUDED_BALXML_HEXPARSER
#define INCLUDED_BALXML_HEXPARSER

#ifndef INCLUDED_BSLS_IDENT
#include <bsls_ident.h>
#endif
BSLS_IDENT("$Id: $")

//@PURPOSE: Provide push parser for hex types.
//
//@CLASSES:
//   balxml::HexParser: push parser for hex types
//
//@SEE_ALSO:
//
//@AUTHOR: Shezan Baig (sbaig)
//
//@CONTACT: Rohan Bhindwale (rbhindwa)
//
//@DESCRIPTION: The 'balxml::HexParser' class template provided by this
// component can be used to parse Hex characters into one of the supported Hex
// types, which are 'bsl::vector<char>' and 'bsl::string'.  The 'TYPE'
// parameter can be one of these two types.
//
// Note that if you need a way to encode binary data into ASCII, the
// 'bdlde_base64encoder' and 'bdlde_base64decoder' components are likely a more
// efficient solution.
//
// This class template is a model of the 'PushParser' concept, which contains
// the following methods:
//..
//  int beginParse(TYPE *object);
//      // Prepare the parser to start parsing a new value and associate the
//      // specified 'object' with the parser.  Return 0 if successful and
//      // non-zero otherwise.
//
//  int endParse();
//      // Ends the parse operation and store the value parsed from the pushed
//      // characters into the associated object.  Return 0 if successful and
//      // non-zero otherwise.  The behavior is undefined unless an object is
//      // associated with this parser.  Upon successful completion, the parser
//      // will be disassociated with the object.
//
//  template <typename INPUT_ITERATOR>
//  int pushCharacters(INPUT_ITERATOR begin, INPUT_ITERATOR end);
//      // Push the characters ranging from the specified 'begin' up to (but
//      // not including) the specified 'end' into this parser.  Return 0 if
//      // successful and non-zero otherwise.  The parameterized
//      // 'INPUT_ITERATOR' must be dereferenceable to a 'char' value.  The
//      // behavior is undefined unless an object is associated with this
//      // parser.
//..
//
///Usage
///-----
// The following snippets of code illustrate the usage of this component.
// Suppose you had an input stream that contained Hex data.  The following
// 'loadFromHexStream' function loads this data into an 'bsl::vector<char>'
// blob:
//..
//  #include <balxml_hexparser.h>
//
//  #include <istream>
//  #include <iterator>
//  #include <vector>
//
//  using namespace BloombergLP;
//
//  int loadFromHexStream(bsl::vector<char> *result, bsl::istream& stream)
//  {
//      enum { k_FAILURE = -1 };
//
//      balxml::HexParser<bsl::vector<char> > parser;
//
//      if (0 != parser.beginParse(result)) {
//          return k_FAILURE;
//      }
//
//      if (0 != parser.pushCharacters(bsl::istreambuf_iterator<char>(stream),
//                                     bsl::istreambuf_iterator<char>())) {
//          return k_FAILURE;
//      }
//
//      return parser.endParse();
//  }
//..
// The following function demonstrates the 'loadFromHexStream' function:
//..
//  #include <sstream>
//
//  void usageExample()
//  {
//      const char INPUT[] = "0F3B296A";
//
//      bsl::vector<char>  vec;
//      bsl::istringstream iss(INPUT);
//
//      int result = loadFromHexStream(&vec, iss);
//
//      assert(0    == result);
//      assert(4    == vec.size());
//      assert(0x0F == vec[0]);
//      assert(0x3B == vec[1]);
//      assert(0x29 == vec[2]);
//      assert(0x6A == vec[3]);
//  }
//..

#ifndef INCLUDED_BALSCM_VERSION
#include <balscm_version.h>
#endif

#ifndef INCLUDED_BDLAT_VALUETYPEFUNCTIONS
#include <bdlat_valuetypefunctions.h>
#endif

#ifndef INCLUDED_BDLB_CHARTYPE
#include <bdlb_chartype.h>
#endif

#ifndef INCLUDED_BSLS_ASSERT
#include <bsls_assert.h>
#endif

namespace BloombergLP {

namespace balxml {
                           // =====================
                           // class HexParser<TYPE>
                           // =====================

template <class TYPE>
class HexParser {
    // This is a push parser for supported Hex types ('bsl::vector<char>' or
    // 'bsl::string').

    // PRIVATE DATA MEMBERS
    char  d_firstDigit;  // buffer for first digit
    TYPE *d_object_p;    // associated object

  private:
    // NOT IMPLEMENTED
    HexParser(const HexParser&);
    HexParser& operator=(const HexParser&);

    // PRIVATE MANIPULATORS
    void appendOctet(char firstDigit, char secondDigit);
        // Append an octet generated from the specified 'firstDigit' and the
        // specified 'secondDigit' to the associated object.

  public:
    // CREATORS
    HexParser();
        // Create a parser for parsing Hex types.

    // Generated by compiler:
    //  ~HexParser();

    // MANIPULATORS
    int beginParse(TYPE *object);
        // Prepare the parser to start parsing a new value and associate the
        // specified 'object' with the parser.  Return 0 if successful and
        // non-zero otherwise.

    int endParse();
        // Ends the parse operation and store the value parsed from the pushed
        // characters into the associated object.  Return 0 if successful and
        // non-zero otherwise.  The behavior is undefined unless an object is
        // associated with this parser.  Upon successful completion, the parser
        // will be disassociated with the object.

    template <class INPUT_ITERATOR>
    int pushCharacters(INPUT_ITERATOR begin, INPUT_ITERATOR end);
        // Push the characters ranging from the specified 'begin' up to (but
        // not including) the specified 'end' into this parser.  Return 0 if
        // successful and non-zero otherwise.  The parameterized
        // 'INPUT_ITERATOR' must be dereferenceable to a 'char' value.  The
        // behavior is undefined unless an object is associated with this
        // parser.
};

                          // =======================
                          // struct HexParser_Helper
                          // =======================

class HexParser_Helper {
    // Namespace for facilities that are used in the implementation of class
    // 'HexParser<TYPE>'.  Only instances of 'HexParser<TYPE>' can access the
    // facilities in this class.

    template <class TYPE> friend class HexParser;

    static const char s_hexValueTable[128];
        // Table that maps from ASCII character value to hex value.
};

// ============================================================================
//                            INLINE DEFINITIONS
// ============================================================================

                           // ---------------------
                           // class HexParser<TYPE>
                           // ---------------------

// PRIVATE MANIPULATORS
template <class TYPE>
void HexParser<TYPE>::appendOctet(char firstDigit, char secondDigit)
{
    BSLS_ASSERT_SAFE(bdlb::CharType::isXdigit((unsigned char) firstDigit));
    BSLS_ASSERT_SAFE(bdlb::CharType::isXdigit((unsigned char) secondDigit));

    char value =
        (char)((HexParser_Helper::s_hexValueTable[(int)firstDigit] << 4)
             | (HexParser_Helper::s_hexValueTable[(int)secondDigit]));

    d_object_p->push_back(value);
}

// CREATORS
template <class TYPE>
HexParser<TYPE>::HexParser()
: d_firstDigit(0)
, d_object_p(0)
{
}

// MANIPULATORS
template <class TYPE>
int HexParser<TYPE>::beginParse(TYPE *object)
{
    BSLS_ASSERT_SAFE(object);

    enum { k_SUCCESS = 0 };

    d_firstDigit = 0;
    d_object_p   = object;

    bdlat_ValueTypeFunctions::reset(d_object_p);

    return k_SUCCESS;
}

template <class TYPE>
int HexParser<TYPE>::endParse()
{
    BSLS_ASSERT_SAFE(d_object_p);

    enum { k_SUCCESS = 0, k_FAILURE = -1 };

    d_object_p = 0;

    return 0 == d_firstDigit ? k_SUCCESS : k_FAILURE;
}

template <class TYPE>
template <class INPUT_ITERATOR>
int HexParser<TYPE>::pushCharacters(INPUT_ITERATOR begin, INPUT_ITERATOR end)
{
    BSLS_ASSERT_SAFE(d_object_p);

    enum { k_SUCCESS = 0, k_FAILURE = -1 };

    while (begin != end) {
        const char digit = *begin;

        ++begin;

        if (!bdlb::CharType::isSpace(digit)) {
            if (!bdlb::CharType::isXdigit(digit)) {
                return k_FAILURE;                                     // RETURN
            }

            if (0 == d_firstDigit) {
                d_firstDigit = digit;
            }
            else {
                appendOctet(d_firstDigit, digit);

                d_firstDigit = 0;
            }
        }
    }

    return k_SUCCESS;
}
}  // close package namespace

}  // close enterprise namespace

#endif

// ----------------------------------------------------------------------------
// Copyright 2015 Bloomberg Finance L.P.
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
