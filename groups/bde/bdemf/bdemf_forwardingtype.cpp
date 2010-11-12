// bdemf_forwardingtype.cpp        -*-C++-*-
#include <bdes_ident.h>
BDES_IDENT_RCSID(bdemf_forwardingtype_cpp,"$Id$ $CSID$")

#include <bdemf_forwardingtype.h>

#include <bdemf_issame.h>          // for testing only
#include <bslmf_isarray.h>         // for testing only
#include <bsls_platform.h>

#if defined(BSLS_PLATFORM__CMP_GNU)
// For g++ 4.3.2 builds on Sun, a completely empty archive causes
//      ld: elf error: file ./libbdemf.dbg_exc_mt.a: elf_getarsym
// errors.  The presence of any "real" symbol in the archive suffices to avoid
// the error, even if it's an unreferenced static.
// Our old RCS ID scheme used to avoid this error unintentionally, by using
// static char arrays for the RCS IDs.
char bdemf_forwardingtype_filename[] = __FILE__;
#endif

// ---------------------------------------------------------------------------
// NOTICE:
//      Copyright (C) Bloomberg L.P., 2005
//      All Rights Reserved.
//      Property of Bloomberg L.P. (BLP)
//      This software is made available solely pursuant to the
//      terms of a BLP license agreement which governs its use.
// ----------------------------- END-OF-FILE ---------------------------------
