#ifndef INCLUDED_BSLS_PLATFORM
#include <bsls_platform.h>
#endif

// This logic is required to block the g++ warning triggered by including
// a "deprecated" header.  The warning is generated by g++'s internal
// backward/backward_warning.h header, so we block its effect by defining its
// include guard.

#if defined(BSLS_PLATFORM_OS_SOLARIS) && defined(BSLS_PLATFORM_CMP_GNU)
#define _BACKWARD_BACKWARD_WARNING_H 1
#endif

#include <bsl_strstream.h>
#include <strstream>
#ifdef std
#   error std was not expected to be a macro
#endif
namespace std { }
int main() { return 0; }

// ----------------------------------------------------------------------------
// NOTICE:
//      Copyright (C) Bloomberg L.P., 2012
//      All Rights Reserved.
//      Property of Bloomberg L.P.  (BLP)
//      This software is made available solely pursuant to the
//      terms of a BLP license agreement which governs its use.
// ----------------------------- END-OF-FILE ----------------------------------
