// baexml_typesprintutil.cpp                  -*-C++-*-
#include <baexml_typesprintutil.h>

#include <bdes_ident.h>
BDES_IDENT_RCSID(baexml_typesprintutil_cpp,"$Id$ $CSID$")

#include <bdede_base64encoder.h>
#include <bdeu_print.h>

#include <bsls_assert.h>
#include <bsls_platform.h>

#include <bsl_iterator.h>

#include <bsl_cfloat.h>
#include <stdio.h>  // for 'sprintf', which is not always in '<bsl_cstdio.h>'

namespace BloombergLP {

namespace {

// HELPER FUNCTIONS

template <typename INPUT_ITERATOR>
bsl::ostream& encodeBase64(bsl::ostream&  stream,
                           INPUT_ITERATOR begin,
                           INPUT_ITERATOR end)
    // Write the base64 encoding of the character sequence defined by the
    // specified 'begin' and 'end' iterators into the specified 'stream' and
    // return 'stream'.  Each instantiation of this private function is is
    // called only once (by the functions below) and can thus be inlined
    // without causing code bloat.
{
    bdede_Base64Encoder base64Encoder(0);  // 0 means do not insert CRLF

    {
        bsl::ostreambuf_iterator<char> outputIterator(stream);

        int status = base64Encoder.convert(outputIterator, begin, end);

        BSLS_ASSERT(0 == status);  // nothing should be retained by encoder
    }

    {
        bsl::ostreambuf_iterator<char> outputIterator(stream);

        int status = base64Encoder.endConvert(outputIterator);

        BSLS_ASSERT(0 == status);  // nothing should be retained by encoder
    }

    return stream;
}

const char *printTextReplacingXMLEscapes(
                                      bsl::ostream&                stream,
                                      const char                  *data,
                                      int                          dataLength,
                                      const baexml_EncoderOptions *options = 0)
    // Format the specified 'data' buffer of the specified 'dataLength' to the
    // specified output 'stream' using the 'bdeat_FormattingMode::BDEAT_TEXT'
    // formatting mode. and using the specified encoder 'options'.  Return 0 on
    // success, and a non-negative value otherwise.  If 'dataLength' is -1,
    // then detect automatically the null character as the end of the string.
    // If 'data' is invalid UTF-8 or contains a null character and
    // 'dataLength' is not -1 (neither one being allowed for XML 1.0), then
    // return the address of the first byte of the offending UTF-8 character
    // in 'data'.
{
    // IMPLEMENTATION NOTE: This implementation does not buffer the output, for
    // two reasons.  The first is that most ostream's are buffered already.
    // The second is that most of the input will be simply copied, in-between
    // two XML escapes, so that the 'stream.write()' method can readily be used
    // on the input buffer.  If there are a lot of XML escapes, the text
    // transcription of these escapes will take time already so the extra time
    // taken by these small 'stream.write()' operations (even if these are
    // virtual function calls) should not unduly slow down the program.

    // For efficiency, we use a table-driven implementation.

    enum {
        PRINTABLE_ASCII = 0,
        TWO_BYTE_SEQUENCE_PREFIX,
        THREE_BYTE_SEQUENCE_PREFIX,
        THREE_BYTE_SEQUENCE_PREFIX_E0,
        THREE_BYTE_SEQUENCE_PREFIX_ED,
        FOUR_BYTE_SEQUENCE_PREFIX,
        FOUR_BYTE_SEQUENCE_PREFIX_F0,
        FOUR_BYTE_SEQUENCE_PREFIX_F4,
        AMPERSAND,
        LESS_THAN,
        GREATER_THAN,
        APOSTROPHE,
        QUOTE,
        CONTROL_CHARACTER,
        UNENCODABLE,
        END_OF_STRING
    };

    static const char ESCAPED_CHARS_TABLE[256] =
    {
        END_OF_STRING,                 //   0   0
        CONTROL_CHARACTER,             //   1   1
        CONTROL_CHARACTER,             //   2   2
        CONTROL_CHARACTER,             //   3   3
        CONTROL_CHARACTER,             //   4   4
        CONTROL_CHARACTER,             //   5   5
        CONTROL_CHARACTER,             //   6   6
        CONTROL_CHARACTER,             //   7   7
        CONTROL_CHARACTER,             //   8   8 - BACKSPACE
        PRINTABLE_ASCII,               //   9   9 - TAB
        PRINTABLE_ASCII,               //  10   a - LF
        CONTROL_CHARACTER,             //  11   b
        CONTROL_CHARACTER,             //  12   c
        PRINTABLE_ASCII,               //  13   d - CR
        CONTROL_CHARACTER,             //  14   e
        CONTROL_CHARACTER,             //  15   f
        CONTROL_CHARACTER,             //  16  10
        CONTROL_CHARACTER,             //  17  11
        CONTROL_CHARACTER,             //  18  12
        CONTROL_CHARACTER,             //  19  13
        CONTROL_CHARACTER,             //  20  14
        CONTROL_CHARACTER,             //  21  15
        CONTROL_CHARACTER,             //  22  16
        CONTROL_CHARACTER,             //  23  17
        CONTROL_CHARACTER,             //  24  18
        CONTROL_CHARACTER,             //  25  19
        CONTROL_CHARACTER,             //  26  1a
        CONTROL_CHARACTER,             //  27  1b
        CONTROL_CHARACTER,             //  28  1c
        CONTROL_CHARACTER,             //  29  1d
        CONTROL_CHARACTER,             //  30  1e
        CONTROL_CHARACTER,             //  31  1f
        PRINTABLE_ASCII,               //  32  20 - SPACE
        PRINTABLE_ASCII,               //  33  21 - !
        QUOTE,                         //  34  22 - "
        PRINTABLE_ASCII,               //  35  23 - #
        PRINTABLE_ASCII,               //  36  24 - $
        PRINTABLE_ASCII,               //  37  25 - %
        AMPERSAND,                     //  38  26 - &
        APOSTROPHE,                    //  39  27 - '
        PRINTABLE_ASCII,               //  40  28 - (
        PRINTABLE_ASCII,               //  41  29 - )
        PRINTABLE_ASCII,               //  42  2a - *
        PRINTABLE_ASCII,               //  43  2b - +
        PRINTABLE_ASCII,               //  44  2c - ,
        PRINTABLE_ASCII,               //  45  2d - -
        PRINTABLE_ASCII,               //  46  2e - .
        PRINTABLE_ASCII,               //  47  2f - /
        PRINTABLE_ASCII,               //  48  30 - 0
        PRINTABLE_ASCII,               //  49  31 - 1
        PRINTABLE_ASCII,               //  50  32 - 2
        PRINTABLE_ASCII,               //  51  33 - 3
        PRINTABLE_ASCII,               //  52  34 - 4
        PRINTABLE_ASCII,               //  53  35 - 5
        PRINTABLE_ASCII,               //  54  36 - 6
        PRINTABLE_ASCII,               //  55  37 - 7
        PRINTABLE_ASCII,               //  56  38 - 8
        PRINTABLE_ASCII,               //  57  39 - 9
        PRINTABLE_ASCII,               //  58  3a - :
        PRINTABLE_ASCII,               //  59  3b - ;
        LESS_THAN,                     //  60  3c - <
        PRINTABLE_ASCII,               //  61  3d - =
        GREATER_THAN,                  //  62  3e - >
        PRINTABLE_ASCII,               //  63  3f - ?
        PRINTABLE_ASCII,               //  64  40 - @
        PRINTABLE_ASCII,               //  65  41 - A
        PRINTABLE_ASCII,               //  66  42 - B
        PRINTABLE_ASCII,               //  67  43 - C
        PRINTABLE_ASCII,               //  68  44 - D
        PRINTABLE_ASCII,               //  69  45 - E
        PRINTABLE_ASCII,               //  70  46 - F
        PRINTABLE_ASCII,               //  71  47 - G
        PRINTABLE_ASCII,               //  72  48 - H
        PRINTABLE_ASCII,               //  73  49 - I
        PRINTABLE_ASCII,               //  74  4a - J
        PRINTABLE_ASCII,               //  75  4b - K
        PRINTABLE_ASCII,               //  76  4c - L
        PRINTABLE_ASCII,               //  77  4d - M
        PRINTABLE_ASCII,               //  78  4e - N
        PRINTABLE_ASCII,               //  79  4f - O
        PRINTABLE_ASCII,               //  80  50 - P
        PRINTABLE_ASCII,               //  81  51 - Q
        PRINTABLE_ASCII,               //  82  52 - R
        PRINTABLE_ASCII,               //  83  53 - S
        PRINTABLE_ASCII,               //  84  54 - T
        PRINTABLE_ASCII,               //  85  55 - U
        PRINTABLE_ASCII,               //  86  56 - V
        PRINTABLE_ASCII,               //  87  57 - W
        PRINTABLE_ASCII,               //  88  58 - X
        PRINTABLE_ASCII,               //  89  59 - Y
        PRINTABLE_ASCII,               //  90  5a - Z
        PRINTABLE_ASCII,               //  91  5b - [
        PRINTABLE_ASCII,               //  92  5c - '\'
        PRINTABLE_ASCII,               //  93  5d - ]
        PRINTABLE_ASCII,               //  94  5e - ^
        PRINTABLE_ASCII,               //  95  5f - _
        PRINTABLE_ASCII,               //  96  60 - `
        PRINTABLE_ASCII,               //  97  61 - a
        PRINTABLE_ASCII,               //  98  62 - b
        PRINTABLE_ASCII,               //  99  63 - c
        PRINTABLE_ASCII,               // 100  64 - d
        PRINTABLE_ASCII,               // 101  65 - e
        PRINTABLE_ASCII,               // 102  66 - f
        PRINTABLE_ASCII,               // 103  67 - g
        PRINTABLE_ASCII,               // 104  68 - h
        PRINTABLE_ASCII,               // 105  69 - i
        PRINTABLE_ASCII,               // 106  6a - j
        PRINTABLE_ASCII,               // 107  6b - k
        PRINTABLE_ASCII,               // 108  6c - l
        PRINTABLE_ASCII,               // 109  6d - m
        PRINTABLE_ASCII,               // 110  6e - n
        PRINTABLE_ASCII,               // 111  6f - o
        PRINTABLE_ASCII,               // 112  70 - p
        PRINTABLE_ASCII,               // 113  71 - q
        PRINTABLE_ASCII,               // 114  72 - r
        PRINTABLE_ASCII,               // 115  73 - s
        PRINTABLE_ASCII,               // 116  74 - t
        PRINTABLE_ASCII,               // 117  75 - u
        PRINTABLE_ASCII,               // 118  76 - v
        PRINTABLE_ASCII,               // 119  77 - w
        PRINTABLE_ASCII,               // 120  78 - x
        PRINTABLE_ASCII,               // 121  79 - y
        PRINTABLE_ASCII,               // 122  7a - z
        PRINTABLE_ASCII,               // 123  7b - {
        PRINTABLE_ASCII,               // 124  7c - |
        PRINTABLE_ASCII,               // 125  7d - }
        PRINTABLE_ASCII,               // 126  7e - ~
        CONTROL_CHARACTER,             // 127  7f - DEL
        UNENCODABLE,                   // 128  80
        UNENCODABLE,                   // 129  81
        UNENCODABLE,                   // 130  82
        UNENCODABLE,                   // 131  83
        UNENCODABLE,                   // 132  84
        UNENCODABLE,                   // 133  85
        UNENCODABLE,                   // 134  86
        UNENCODABLE,                   // 135  87
        UNENCODABLE,                   // 136  88
        UNENCODABLE,                   // 137  89
        UNENCODABLE,                   // 138  8a
        UNENCODABLE,                   // 139  8b
        UNENCODABLE,                   // 140  8c
        UNENCODABLE,                   // 141  8d
        UNENCODABLE,                   // 142  8e
        UNENCODABLE,                   // 143  8f
        UNENCODABLE,                   // 144  90
        UNENCODABLE,                   // 145  91
        UNENCODABLE,                   // 146  92
        UNENCODABLE,                   // 147  93
        UNENCODABLE,                   // 148  94
        UNENCODABLE,                   // 149  95
        UNENCODABLE,                   // 150  96
        UNENCODABLE,                   // 151  97
        UNENCODABLE,                   // 152  98
        UNENCODABLE,                   // 153  99
        UNENCODABLE,                   // 154  9a
        UNENCODABLE,                   // 155  9b
        UNENCODABLE,                   // 156  9c
        UNENCODABLE,                   // 157  9d
        UNENCODABLE,                   // 158  9e
        UNENCODABLE,                   // 159  9f
        UNENCODABLE,                   // 160  a0
        UNENCODABLE,                   // 161  a1
        UNENCODABLE,                   // 162  a2
        UNENCODABLE,                   // 163  a3
        UNENCODABLE,                   // 164  a4
        UNENCODABLE,                   // 165  a5
        UNENCODABLE,                   // 166  a6
        UNENCODABLE,                   // 167  a7
        UNENCODABLE,                   // 168  a8
        UNENCODABLE,                   // 169  a9
        UNENCODABLE,                   // 170  aa
        UNENCODABLE,                   // 171  ab
        UNENCODABLE,                   // 172  ac
        UNENCODABLE,                   // 173  ad
        UNENCODABLE,                   // 174  ae
        UNENCODABLE,                   // 175  af
        UNENCODABLE,                   // 176  b0
        UNENCODABLE,                   // 177  b1
        UNENCODABLE,                   // 178  b2
        UNENCODABLE,                   // 179  b3
        UNENCODABLE,                   // 180  b4
        UNENCODABLE,                   // 181  b5
        UNENCODABLE,                   // 182  b6
        UNENCODABLE,                   // 183  b7
        UNENCODABLE,                   // 184  b8
        UNENCODABLE,                   // 185  b9
        UNENCODABLE,                   // 186  ba
        UNENCODABLE,                   // 187  bb
        UNENCODABLE,                   // 188  bc
        UNENCODABLE,                   // 189  bd
        UNENCODABLE,                   // 190  be
        UNENCODABLE,                   // 191  bf
        UNENCODABLE,                   // 192  c0
        UNENCODABLE,                   // 193  c1
        TWO_BYTE_SEQUENCE_PREFIX,      // 194  c2
        TWO_BYTE_SEQUENCE_PREFIX,      // 195  c3
        TWO_BYTE_SEQUENCE_PREFIX,      // 196  c4
        TWO_BYTE_SEQUENCE_PREFIX,      // 197  c5
        TWO_BYTE_SEQUENCE_PREFIX,      // 198  c6
        TWO_BYTE_SEQUENCE_PREFIX,      // 199  c7
        TWO_BYTE_SEQUENCE_PREFIX,      // 200  c8
        TWO_BYTE_SEQUENCE_PREFIX,      // 201  c9
        TWO_BYTE_SEQUENCE_PREFIX,      // 202  ca
        TWO_BYTE_SEQUENCE_PREFIX,      // 203  cb
        TWO_BYTE_SEQUENCE_PREFIX,      // 204  cc
        TWO_BYTE_SEQUENCE_PREFIX,      // 205  cd
        TWO_BYTE_SEQUENCE_PREFIX,      // 206  ce
        TWO_BYTE_SEQUENCE_PREFIX,      // 207  cf
        TWO_BYTE_SEQUENCE_PREFIX,      // 208  d0
        TWO_BYTE_SEQUENCE_PREFIX,      // 209  d1
        TWO_BYTE_SEQUENCE_PREFIX,      // 210  d2
        TWO_BYTE_SEQUENCE_PREFIX,      // 211  d3
        TWO_BYTE_SEQUENCE_PREFIX,      // 212  d4
        TWO_BYTE_SEQUENCE_PREFIX,      // 213  d5
        TWO_BYTE_SEQUENCE_PREFIX,      // 214  d6
        TWO_BYTE_SEQUENCE_PREFIX,      // 215  d7
        TWO_BYTE_SEQUENCE_PREFIX,      // 216  d8
        TWO_BYTE_SEQUENCE_PREFIX,      // 217  d9
        TWO_BYTE_SEQUENCE_PREFIX,      // 218  da
        TWO_BYTE_SEQUENCE_PREFIX,      // 219  db
        TWO_BYTE_SEQUENCE_PREFIX,      // 220  dc
        TWO_BYTE_SEQUENCE_PREFIX,      // 221  dd
        TWO_BYTE_SEQUENCE_PREFIX,      // 222  de
        TWO_BYTE_SEQUENCE_PREFIX,      // 223  df
        THREE_BYTE_SEQUENCE_PREFIX_E0, // 224  e0
        THREE_BYTE_SEQUENCE_PREFIX,    // 225  e1
        THREE_BYTE_SEQUENCE_PREFIX,    // 226  e2
        THREE_BYTE_SEQUENCE_PREFIX,    // 227  e3
        THREE_BYTE_SEQUENCE_PREFIX,    // 228  e4
        THREE_BYTE_SEQUENCE_PREFIX,    // 229  e5
        THREE_BYTE_SEQUENCE_PREFIX,    // 230  e6
        THREE_BYTE_SEQUENCE_PREFIX,    // 231  e7
        THREE_BYTE_SEQUENCE_PREFIX,    // 232  e8
        THREE_BYTE_SEQUENCE_PREFIX,    // 233  e9
        THREE_BYTE_SEQUENCE_PREFIX,    // 234  ea
        THREE_BYTE_SEQUENCE_PREFIX,    // 235  eb
        THREE_BYTE_SEQUENCE_PREFIX,    // 236  ec
        THREE_BYTE_SEQUENCE_PREFIX_ED, // 237  ed
        THREE_BYTE_SEQUENCE_PREFIX,    // 238  ee
        THREE_BYTE_SEQUENCE_PREFIX,    // 239  ef
        FOUR_BYTE_SEQUENCE_PREFIX_F0,  // 240  f0
        FOUR_BYTE_SEQUENCE_PREFIX,     // 241  f1
        FOUR_BYTE_SEQUENCE_PREFIX,     // 242  f2
        FOUR_BYTE_SEQUENCE_PREFIX,     // 243  f3
        FOUR_BYTE_SEQUENCE_PREFIX_F4,  // 244  f4
        UNENCODABLE,                   // 245  f5
        UNENCODABLE,                   // 246  f6
        UNENCODABLE,                   // 247  f7
        UNENCODABLE,                   // 248  f8
        UNENCODABLE,                   // 249  f9
        UNENCODABLE,                   // 250  fa
        UNENCODABLE,                   // 251  fb
        UNENCODABLE,                   // 252  fc
        UNENCODABLE,                   // 253  fd
        UNENCODABLE,                   // 254  fe
        UNENCODABLE,                   // 255  ff
    };

    const char *runBegin = data;
    const char *end      = (dataLength >= 0) ? data + dataLength
                                             : (const char *)-1;

    while (data < end) {
        // The logic is to test if the current character should interrupt the
        // current run of printable characters and, if so, flush the run
        // to the stream, output the special character, and continue.
        // For efficiency, we replicate the calls to 'stream.write(...)' in
        // order to perform a single test per character.

        switch (ESCAPED_CHARS_TABLE[(unsigned char)*data]) {
          case END_OF_STRING: {
            // If 'dataLength' is -1, then this character is accepted, the
            // current run is flushed to the stream, and we should return
            // success.  Otherwise we should treat as unencodable.

            if (dataLength == -1) {
                stream.write(runBegin, data - runBegin);
                return 0;                                             // RETURN
            }                                              // ELSE FALL THROUGH
          }

          case CONTROL_CHARACTER: {
            // Control characters, although allowed in XML 1.1, are discouraged
            // in XML 1.0.  But we allow them if the encoding options
            // explicitly say so.

            if (!options || !options->allowControlCharacters()) {
                stream.write(runBegin, data - runBegin);
                stream.setstate(bsl::ios_base::failbit);
                return data;  // error position                          RETURN
            }
            ++data;
          } break;
          case UNENCODABLE: {
            // Unencodable characters are simply not allowed.  Flush current
            // run and exit.

            stream.write(runBegin, data - runBegin);
            stream.setstate(bsl::ios_base::failbit);
            return data;  // error position                              RETURN
          }

          case AMPERSAND: {
            stream.write(runBegin, data - runBegin);
            static const char amp[] = "&amp;";
            stream.write(amp, sizeof(amp) - 1);
            runBegin = ++data;
          } break;

          case LESS_THAN: {
            stream.write(runBegin, data - runBegin);
            static const char lt[] = "&lt;";
            stream.write(lt, sizeof(lt) - 1);
            runBegin = ++data;
          } break;

          case GREATER_THAN: {
            stream.write(runBegin, data - runBegin);
            static const char gt[] = "&gt;";
            stream.write(gt, sizeof(gt) - 1);
            runBegin = ++data;
          } break;

          case APOSTROPHE: {
            stream.write(runBegin, data - runBegin);
            static const char apos[] = "&apos;";
            stream.write(apos, sizeof(apos) - 1);
            runBegin = ++data;
          } break;

          case QUOTE: {
            stream.write(runBegin, data - runBegin);
            static const char quot[] = "&quot;";
            stream.write(quot, sizeof(quot) - 1);
            runBegin = ++data;
          } break;

          // In remaining cases, including default, do not interrupt current
          // run, in other words, do not issue:
          //..
          //  stream.write(runBegin, data - runBegin);
          //..
          // Simply move on while keeping 'runBegin' pegged to the beginning
          // of the run.

          case TWO_BYTE_SEQUENCE_PREFIX: {
            // Second byte should be in the range 0x80..0xbf.  If not,
            // including if it is the null-terminating character,
            // flush current run and exit.

            if (!(data + 1 < end) || (*(data + 1) & 0xc0) != 0x80) {
                stream.write(runBegin, data - runBegin);
                stream.setstate(bsl::ios_base::failbit);
                return data;  // error position                          RETURN
            }
            data += 2;
          } break;

          case THREE_BYTE_SEQUENCE_PREFIX: {
            // Second and third bytes should be in the range 0x80..0xbf.
            // If not, including if the null-terminating character is
            // encountered, flush current run and exit.

            if (!(data + 2 < end) || (*(data + 1) & 0xc0) != 0x80
                                  || (*(data + 2) & 0xc0) != 0x80) {
                stream.write(runBegin, data - runBegin);
                stream.setstate(bsl::ios_base::failbit);
                return data;  // error position                          RETURN
            }
            data += 3;
          } break;

          case THREE_BYTE_SEQUENCE_PREFIX_E0: {
            // Second byte should be in the range 0xa0..0xbf.
            // Third bytes should be in the range 0x80..0xbf.
            // If not, including if the null-terminating character is
            // encountered, flush current run and exit.

            if (!(data + 2 < end) || (*(data + 1) & 0xe0) != 0xa0
                                  || (*(data + 2) & 0xc0) != 0x80) {
                stream.write(runBegin, data - runBegin);
                stream.setstate(bsl::ios_base::failbit);
                return data;  // error position                          RETURN
            }
            data += 3;
          } break;

          case THREE_BYTE_SEQUENCE_PREFIX_ED: {
            // Second byte should be in the range 0x80..0x9f
            // Third bytes should be in the range 0x80..0xbf
            // If not, including if the null-terminating character is
            // encountered, flush current run and exit.

            if (!(data + 2 < end) || (*(data + 1) & 0xe0) != 0x80
                                  || (*(data + 2) & 0xc0) != 0x80) {
                stream.write(runBegin, data - runBegin);
                stream.setstate(bsl::ios_base::failbit);
                return data;  // error position                          RETURN
            }
            data += 3;
          } break;

          case FOUR_BYTE_SEQUENCE_PREFIX: {
            // Second to fourth bytes should be in the range 0x80..0xbf
            // If not, including if the null-terminating character is
            // encountered, flush current run and exit.

            if (!(data + 3 < end) || (*(data + 1) & 0xc0) != 0x80
                                  || (*(data + 2) & 0xc0) != 0x80
                                  || (*(data + 3) & 0xc0) != 0x80) {
                stream.write(runBegin, data - runBegin);
                stream.setstate(bsl::ios_base::failbit);
                return data;  // error position                          RETURN
            }
            data += 4;
          } break;

          case FOUR_BYTE_SEQUENCE_PREFIX_F0: {
            // Second byte should be in the range 0x90..0xbf
            // Third and fourth bytes should be in the range 0x80..0xbf
            // If not, including if the null-terminating character is
            // encountered, flush current run and exit.

            if (!(data + 3 < end)
             || !((*(data + 1) & 0xc0) == 0x80 && (*(data + 1) & 0x90) != 0x00)
             || (*(data + 2) & 0xc0) != 0x80 || (*(data + 3) & 0xc0) != 0x80) {
                stream.write(runBegin, data - runBegin);
                stream.setstate(bsl::ios_base::failbit);
                return data;  // error position                          RETURN
            }
            data += 4;
          } break;

          case FOUR_BYTE_SEQUENCE_PREFIX_F4: {
            // Second byte should be in the range 0x80..0x8f
            // Third and fourth bytes should be in the range 0x80..0xbf
            // If not, including if the null-terminating character is
            // encountered, flush current run and exit.

            if (!(data + 3 < end) || (*(data + 1) & 0xf0) != 0x80
                                  || (*(data + 2) & 0xc0) != 0x80
                                  || (*(data + 3) & 0xc0) != 0x80) {
                stream.write(runBegin, data - runBegin);
                stream.setstate(bsl::ios_base::failbit);
                return data;  // error position                          RETURN
            }
            data += 4;
          } break;

          case PRINTABLE_ASCII:
          default: {
            ++data;
          } break;
        }
    }

    // Flush the rest of the printable characters in the buffer to stream.

    stream.write(runBegin, data - runBegin);
    return 0;
}

bsl::ostream& printDecimalImpl(bsl::ostream& stream,
                               double        object,
                               int           precision)
{
    switch (bdes_Float::classifyFine(object)) {
      case bdes_Float::BDES_POSITIVE_INFINITY:
      case bdes_Float::BDES_NEGATIVE_INFINITY:
      case bdes_Float::BDES_QNAN:
      case bdes_Float::BDES_SNAN: {
        stream.setstate(bsl::ios_base::failbit);
        return stream;
      }
      default: {
      } break;
    }

    BSLS_ASSERT(precision <= DBL_DIG);

    // On Sun, formatting a 'double' using 'ostream' is 60% slower than
    // 'sprintf', even though the former calls the latter.  Also, note that
    // 'snprintf' is 15% slower than 'sprintf' on Sun.

    char buffer[DBL_MAX_10_EXP + DBL_DIG + 4];  // buffer with headroom

#if defined(BSLS_PLATFORM__CMP_MSVC)
#pragma warning(push)
#pragma warning(disable:4996)     // suppress 'sprintf' deprecation warning
#endif

    // format: "-"  forces left alignment, "#" always prints period
    const int len = ::sprintf(buffer, "%-#1.*f", precision, object);
    BSLS_ASSERT(len < (int) sizeof buffer);

#if defined(BSLS_PLATFORM__CMP_MSVC)
#pragma warning(pop)
#endif

    const char *ptr = bsl::strchr(buffer, '.');
    BSLS_ASSERT(ptr != 0);

    int integralLen = ptr - buffer;

    if (buffer[0] == '-') {
        --integralLen;
    }

    // TBD the significance of "3" is not clear

    int maxFractionLen = (precision + 3) - integralLen;
    if (maxFractionLen <= 0) {
        maxFractionLen = 1;
    }

    ++ptr;  // move ptr to the first fraction digit

    int fractionLen = bsl::strlen(ptr);

    if (fractionLen > maxFractionLen) {
        fractionLen = maxFractionLen;
    }

    stream.write(buffer, (ptr - buffer) + fractionLen);

    return stream;
}

}  // close unnamed namespace

                         // --------------------------------
                         // struct baexml_TypesPrintUtil_Imp
                         // --------------------------------

// BASE64 FUNCTIONS

bsl::ostream&
baexml_TypesPrintUtil_Imp::printBase64(bsl::ostream&                stream,
                                       const bsl::string&           object,
                                       const baexml_EncoderOptions *,
                                       bdeat_TypeCategory::Simple)
{
    // Calls a function in the unnamed namespace.  Cannot be inlined.
    return encodeBase64(stream, object.begin(), object.end());
}

bsl::ostream&
baexml_TypesPrintUtil_Imp::printBase64(bsl::ostream&                 stream,
                                       const bdeut_StringRef&        object,
                                       const baexml_EncoderOptions  *,
                                       bdeat_TypeCategory::Simple)
{
    // Calls a function in the unnamed namespace.  Cannot be inlined.
    return encodeBase64(stream, object.begin(), object.end());
}

bsl::ostream&
baexml_TypesPrintUtil_Imp::printBase64(bsl::ostream&                stream,
                                       const bsl::vector<char>&     object,
                                       const baexml_EncoderOptions *,
                                       bdeat_TypeCategory::Array)
{
    // Calls a function in the unnamed namespace.  Cannot be inlined.
    return encodeBase64(stream, object.begin(), object.end());
}

// HEX FUNCTIONS

bsl::ostream&
baexml_TypesPrintUtil_Imp::printHex(bsl::ostream&                stream,
                                    const bsl::string&           object,
                                    const baexml_EncoderOptions *,
                                    bdeat_TypeCategory::Simple)
{
    return bdeu_Print::singleLineHexDump(stream, object.begin(), object.end());
}

bsl::ostream&
baexml_TypesPrintUtil_Imp::printHex(bsl::ostream&                 stream,
                                    const bdeut_StringRef&        object,
                                    const baexml_EncoderOptions  *,
                                    bdeat_TypeCategory::Simple)
{
    return bdeu_Print::singleLineHexDump(stream, object.begin(), object.end());
}

bsl::ostream&
baexml_TypesPrintUtil_Imp::printHex(bsl::ostream&                   stream,
                                    const bsl::vector<char>&        object,
                                    const baexml_EncoderOptions    *,
                                    bdeat_TypeCategory::Array)
{
    return bdeu_Print::singleLineHexDump(stream, object.begin(), object.end());
}

// TEXT FUNCTIONS

bsl::ostream&
baexml_TypesPrintUtil_Imp::printText(bsl::ostream&                stream,
                                     const char&                  object,
                                     const baexml_EncoderOptions *options,
                                     bdeat_TypeCategory::Simple)
{
    // Calls a function in the unnamed namespace.  Cannot be inlined.
    printTextReplacingXMLEscapes(stream, &object, 1, options);
    return stream;
}

bsl::ostream&
baexml_TypesPrintUtil_Imp::printText(bsl::ostream&                stream,
                                     const char                  *object,
                                     const baexml_EncoderOptions *options,
                                     bdeat_TypeCategory::Simple)
{
    // Calls a function in the unnamed namespace.  Cannot be inlined.
    printTextReplacingXMLEscapes(stream, object, -1, options);
    return stream;
}

bsl::ostream&
baexml_TypesPrintUtil_Imp::printText(bsl::ostream&                stream,
                                     const bsl::string&           object,
                                     const baexml_EncoderOptions *options,
                                     bdeat_TypeCategory::Simple)
{
    // Calls a function in the unnamed namespace.  Cannot be inlined.
    printTextReplacingXMLEscapes(stream,
                                 object.data(),
                                 object.length(),
                                 options);
    return stream;
}

bsl::ostream&
baexml_TypesPrintUtil_Imp::printText(bsl::ostream&                stream,
                                     const bdeut_StringRef&       object,
                                     const baexml_EncoderOptions *options,
                                     bdeat_TypeCategory::Simple)
{
    // Calls a function in the unnamed namespace.  Cannot be inlined.
    printTextReplacingXMLEscapes(stream,
                                 object.data(),
                                 object.length(),
                                 options);
    return stream;
}

bsl::ostream&
baexml_TypesPrintUtil_Imp::printText(bsl::ostream&                stream,
                                     const bsl::vector<char>&     object,
                                     const baexml_EncoderOptions *options,
                                     bdeat_TypeCategory::Array)
{
    // Calls a function in the unnamed namespace.  Cannot be inlined.

    // This 'if' statement prevents us from invoking undefined behavior by
    // taking the address of the first element of an empty vector.
    if (! object.empty()) {
        printTextReplacingXMLEscapes(stream,
                                     &object[0],
                                     object.size(),
                                     options);
    }
    return stream;
}

bsl::ostream&
baexml_TypesPrintUtil_Imp::printDecimal(bsl::ostream&                stream,
                                        const float&                 object,
                                        const baexml_EncoderOptions *,
                                        bdeat_TypeCategory::Simple)
{
    return printDecimalImpl(stream, object, FLT_DIG);
}

bsl::ostream&
baexml_TypesPrintUtil_Imp::printDecimal(bsl::ostream&                stream,
                                        const double&                object,
                                        const baexml_EncoderOptions *,
                                        bdeat_TypeCategory::Simple)
{
    return printDecimalImpl(stream, object, DBL_DIG);
}

bsl::ostream& baexml_TypesPrintUtil_Imp::printDefault(
                                           bsl::ostream&                stream,
                                           const float&                 object,
                                           const baexml_EncoderOptions *,
                                           bdeat_TypeCategory::Simple)
{
    switch (bdes_Float::classifyFine(object)) {
      case bdes_Float::BDES_POSITIVE_INFINITY: {
        stream << "+INF";
      } break;
      case bdes_Float::BDES_NEGATIVE_INFINITY: {
        stream << "-INF";
      } break;
      case bdes_Float::BDES_QNAN:
      case bdes_Float::BDES_SNAN: {
        stream << "NaN";
      } break;
      default: {
        // not a NaN and not +/- INFINITY

        // On Sun, formatting a 'double' using 'ostream' is 60% slower than
        // 'sprintf', even though the former calls the latter.  Also, note that
        // 'snprintf' is 15% slower than 'sprintf' on Sun.

        char buffer[FLT_DIG + 20];  // buffer with headroom

#if defined(BSLS_PLATFORM__CMP_MSVC)
#pragma warning(push)
#pragma warning(disable:4996)     // suppress 'sprintf' deprecation warning
#endif

        const int len = ::sprintf(buffer, "%.*g", FLT_DIG + 1,
                                     (double) object);
        BSLS_ASSERT(len < (int) sizeof buffer);

#if defined(BSLS_PLATFORM__CMP_MSVC)
#pragma warning(pop)
#endif

        stream.write(buffer, len);
      } break;
    }

    return stream;
}

bsl::ostream& baexml_TypesPrintUtil_Imp::printDefault(
                                           bsl::ostream&                stream,
                                           const double&                object,
                                           const baexml_EncoderOptions *,
                                           bdeat_TypeCategory::Simple)
{
    switch (bdes_Float::classifyFine(object)) {
      case bdes_Float::BDES_POSITIVE_INFINITY: {
        stream << "+INF";
      } break;
      case bdes_Float::BDES_NEGATIVE_INFINITY: {
        stream << "-INF";
      } break;
      case bdes_Float::BDES_QNAN:
      case bdes_Float::BDES_SNAN: {
        stream << "NaN";
      } break;
      default: {
        // not a NaN and not +/- INFINITY

        // On Sun, formatting a 'double' using 'ostream' is 60% slower than
        // 'sprintf', even though the former calls the latter.  Also, note that
        // 'snprintf' is 15% slower than 'sprintf' on Sun.

        char buffer[DBL_DIG + 20];  // buffer with headroom

#if defined(BSLS_PLATFORM__CMP_MSVC)
#pragma warning(push)
#pragma warning(disable:4996)     // suppress 'sprintf' deprecation warning
#endif

        const int len = ::sprintf(buffer, "%.*g", DBL_DIG + 1, object);
        BSLS_ASSERT(len < (int) sizeof buffer);

#if defined(BSLS_PLATFORM__CMP_MSVC)
#pragma warning(pop)
#endif

        stream.write(buffer, len);
      } break;
    }

    return stream;
}

// TBD obsolete code brought over from .h when the above two 'printDefault'
// methods were moved to .cpp
#if 0
    // if this double corresponds to a xs:double xml type
    //     store the current formatting flags
    //     set uppercase and scientific formatting flag
    //     set precision to significantDigits
    //     reset the original formatting flags
    //     // Ignore the fixed formatting mode
    // else if the double corresponds to a xs:decimal
    //     snprintf out to a character buffer
    //     calculate the integralLen and fractionLen
    //     print out the value based on the following algorithm:
    //     1. if the integral digits are greater than maxTotalDigits,
    //        print out all integral digits, truncation fraction digits as
    //        necessary
    //     2. print out the smallest number out of maxFractionDigits,
    //        maxTotalDigits - integralLen and fractionLen of fraction digits.
    //     3. special case '-' sign and do not use it when calculating the
    //        integral digits to print

    int maxTotalDigits    = encoderOptions->maxDecimalTotalDigits();
    int maxFractionDigits = encoderOptions->maxDecimalFractionDigits();
    int significantDigits = encoderOptions->significantDoubleDigits();

    // TBD differentiate between double and decimal
    // TBD implement significantDoubleDigits for double
    // TBD what is a safe size for the double buffer ?
    const int bufSize = 320;
    char buffer[bufSize];
    bsl::memset(buffer, 0, bufSize);

    snprintf(buffer, sizeof(buffer), "%f", object);
    const char *decimalLoc = buffer;
    while (*decimalLoc && *decimalLoc != '.') {
        ++decimalLoc;
    }

    int integralLen = decimalLoc - buffer;
    int fractionLen = !*decimalLoc ? 0 : bsl::strlen(decimalLoc + 1);
    int length      = 0;

    if ('-' == buffer[0]) {
        ++maxTotalDigits;
    }

    const char *end = decimalLoc + fractionLen;
    while (end != decimalLoc && *end == '0') {
        // Remove redundant 0s after the decimal point
        // TBD do we want to do this.  Is there a formatting flag to print to
        // disable printing of trailing 0s.
        --end;
        --fractionLen;
    }

    if (integralLen >= maxTotalDigits) {
        length = integralLen;
    }
    else {
        int fractionLenToPrint = maxTotalDigits - integralLen;
        fractionLenToPrint     = fractionLenToPrint <= maxFractionDigits
                               ? fractionLenToPrint
                               : maxFractionDigits;
        fractionLenToPrint = fractionLenToPrint <= fractionLen
                           ? fractionLenToPrint
                           : fractionLen;

        length = 0 == fractionLenToPrint
               ? integralLen
               : integralLen + 1 + fractionLenToPrint;
    }

    if (2 == length && '-' == buffer[0] && '0' == buffer[1]) {
        // Output -0 as 0
        stream.put('0');
    }
    else {
        stream.write(buffer, length);
    }

    return stream;
#endif

}  // close namespace BloombergLP

// ---------------------------------------------------------------------------
// NOTICE:
//      Copyright (C) Bloomberg L.P., 2007
//      All Rights Reserved.
//      Property of Bloomberg L.P. (BLP)
//      This software is made available solely pursuant to the
//      terms of a BLP license agreement which governs its use.
// ----------------------------- END-OF-FILE ---------------------------------
