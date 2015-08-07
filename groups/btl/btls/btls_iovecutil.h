// btls_iovecutil.h                                                   -*-C++-*-
#ifndef INCLUDED_BTLS_IOVECUTIL
#define INCLUDED_BTLS_IOVECUTIL

#ifndef INCLUDED_BSLS_IDENT
#include <bsls_ident.h>
#endif
BSLS_IDENT("$Id: $")

//@PURPOSE: Provide utilities to manipulate iovec structures.
//
//@AUTHOR: Andrei Basov (abasov)
//
//@CLASSES:
//  btls::IovecUtil: namespace for iovec manipulation utilities
//
//@SEE_ALSO: btls_iovec
//
//@DESCRIPTION:  This component provides a namespace for several utilities for
// accessing and manipulating sequences of 'btls::Iovec' and 'btls::Ovec'
// buffers, including 'scatter'-ing ('gather'-ing) a single continuous buffer
// into (from) a sequence of buffers, computing the total length of a sequence
// of buffers, and rewriting the data from a sequence of buffers into a
// 'bdlmca::Blob' object.
//
///Thread-safety
///-------------
// It is safe to invoke methods of this utility with distinct instances of
// their arguments in different threads.  It is safe to access a 'bdlmca::Blob'
// object within different methods of this utility from different threads if no
// method modifies the contents of that object.  It is not safe to access or
// modify a 'bdlmca::Blob' object from any method of this utility while a
// method modifies the content of this object.
//
///Performance
///-----------
// Computing 'length' takes time proportional to the number of buffers in the
// sequence.  All other methods take time proportional to the total length of
// the buffers in the sequence.
//
///Usage
///-----
//..
//..

#ifndef INCLUDED_BTLSCM_VERSION
#include <btlscm_version.h>
#endif

#ifndef INCLUDED_BTLS_IOVEC
#include <btls_iovec.h>
#endif

                            // =====================
                            // class btls::IovecUtil
                            // =====================

namespace BloombergLP {

namespace bslma  { class Allocator; }

namespace bdlmca { class Blob; }
namespace bdlmca { class BlobBufferFactory; }

namespace btls {

struct IovecUtil {
    // This 'struct' provides a namespace for several utilities for
    // manipulating 'Iovecs', including scatter/gather from a single continuous
    // buffer into an iovec structure, computing the length of an iovec
    // structure, and rewriting the data in an iovec structure into a
    // 'bdlmca::Blob'.

    static void appendToBlob(bdlmca::Blob *blob,
                             const Iovec  *buffers,
                             int           numBuffers,
                             int           offset = 0);
    static void appendToBlob(bdlmca::Blob *blob,
                             const Ovec   *buffers,
                             int           numBuffers,
                             int           offset = 0);
        // Append to the specified 'blob' the data in the specified sequence of
        // 'buffers' of the specified 'numBuffers' length.  Optionally specify
        // an 'offset' (in bytes) in the data contained in the 'buffers' to
        // indicate the start of the appended data; if offset is not specified,
        // then start at offset 0 (i.e., use all the 'buffers').  Upon return,
        // the length of the 'blob' will be increased by the number of
        // characters appended, and the data contained in the 'blob' will be a
        // concatenation of the data contained in the buffers from 'offset'
        // until the end.  The behavior is undefined unless
        // '0 <= offset < totalDataSize' and either
        // 'blob.length() + totalDataSize <= blob.totalSize()' or 'blob' was
        // created with a blob buffer factory where 'totalDataSize' represents
        // the total size of the data in 'buffers'.

    static bdlmca::Blob *blob(const Iovec               *buffers,
                              int                        numBuffers,
                              bdlmca::BlobBufferFactory *factory,
                              bslma::Allocator          *basicAllocator = 0);
    static bdlmca::Blob *blob(const Iovec               *buffers,
                              int                        numBuffers,
                              int                        offset,
                              bdlmca::BlobBufferFactory *factory,
                              bslma::Allocator          *basicAllocator = 0);
        // Create a 'bdlmca::Blob' object allocated using the specified
        // 'factory' and containing data from the specified sequence of
        // 'buffers' of the specified 'numBuffers' length.  Optionally specify
        // an 'offset' (in bytes) in the data contained in the 'buffers' to
        // indicate the start of the copied data; if offset is not specified,
        // then start at offset 0 (i.e., use all the 'buffers').  Optionally
        // specify 'basicAllocator' used to supply memory.  If 'basicAllocator'
        // is 0, the currently installed default allocator is used.  Return a
        // pointer to the created blob.  The behavior is undefined unless
        // '0 <= offset < totalDataSize' where 'totalDataSize' represents the
        // total size of the data in 'buffers'.

    static int gather(char        *buffer,
                      int          length,
                      const Iovec *buffers,
                      int          numBuffers);
    static int gather(char        *buffer,
                      int          length,
                      const Ovec  *buffers,
                      int          numBuffers);
        // Copy into the specified 'buffer' of the specified 'length' data from
        // the specified sequence of 'buffers' of the specified 'numBuffers'
        // length.  Return the total number of bytes copied.  The behavior is
        // undefined unless '0 <= length' and '0 <= numBuffers'.

    static int length(const Iovec *buffers, int numBuffers);
    static int length(const Ovec  *buffers, int numBuffers);
        // Return the total length of data in bytes contained in the specified
        // sequence of 'buffers' of the specified 'numBuffers' length.  The
        // behavior is undefined unless '0 <= numBuffers'.

    static void pivot(int         *bufferIdx,
                      int         *offset,
                      const Iovec *buffers,
                      int          numBuffers,
                      int          position);
    static void pivot(int         *bufferIdx,
                      int         *offset,
                      const Ovec  *buffers,
                      int          numBuffers,
                      int          position);
        // Load into the specified 'bufferIdx' the index of the buffer and into
        // the specified 'offset' the offset within that buffer of the byte
        // corresponding to the specified 'position' in the specified sequence
        // of 'buffers' of the specified 'numBuffers' length.  The behavior is
        // undefined unless '0 <= numBuffers' and '0 <= position'.  Note that
        // if 'position' is greater than the total length of the 'buffers',
        // 'bufferIdx' is set to 'numBuffers', and 'offset' is not changed.

    static int scatter(const Iovec *buffers,
                       int          numBuffers,
                       const char  *buffer,
                       int          length);
    static int scatter(const Ovec  *buffers,
                       int          numBuffers,
                       const char  *buffer,
                       int          length);
        // Copy into the specified sequence of 'buffers' of the specified
        // 'numBuffers' length the data from the specified 'buffer' of the
        // specified 'length'.  Return the total number of bytes copied.  The
        // behavior is undefined unless '0 <= numBuffers' and '0 <= length'.
};

//-----------------------------------------------------------------------------
//                      INLINE FUNCTION DEFINITIONS
//-----------------------------------------------------------------------------

                        // ---------------
                        // class IovecUtil
                        // ---------------

inline
bdlmca::Blob *IovecUtil::blob(const Iovec               *vecs,
                              int                        numVecs,
                              bdlmca::BlobBufferFactory *factory,
                              bslma::Allocator          *basicAllocator)
{
    return blob(vecs, numVecs, 0, factory, basicAllocator);
}

}  // close package namespace

}  // close namespace BloombergLP

#endif

// ---------------------------------------------------------------------------
// NOTICE:
//      Copyright (C) Bloomberg L.P., 2015
//      All Rights Reserved.
//      Property of Bloomberg L.P. (BLP)
//      This software is made available solely pursuant to the
//      terms of a BLP license agreement which governs its use.
// ----------------------------- END-OF-FILE ---------------------------------
