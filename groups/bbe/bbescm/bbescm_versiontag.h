// bbescm_versiontag.h -*-C++-*-
#ifndef INCLUDED_BBESCM_VERSIONTAG
#define INCLUDED_BBESCM_VERSIONTAG

#ifndef INCLUDED_BDES_IDENT
#include <bdes_ident.h>
#endif
BDES_IDENT("$Id: $")

//@PURPOSE: Provide versioning information for the 'bbe' package group.
//
//@SEE_ALSO: bbescm_version
//
//@AUTHOR: Pablo Halpern (phalpern), Bill Chapman (bchapman2)
//
//@DESCRIPTION: This component provides versioning information for the 'bbe'
// package group.  The 'BBE_VERSION' macro that is supplied can be used for
// conditional-compilation based on 'bbe' version information.  The following
// usage example illustrates this basic capability.
//
///Usage
///-----
// At compile time, the version of BBE can be used to select an older or newer
// way to accomplish a task, to enable new functionality, or to accommodate an
// interface change.  For example, if the name of a function changes (a rare
// occurrence, but potentially disruptive when it does happen), the impact on
// affected code can be minimized by conditionally calling the function by its
// old or new name using conditional compilation.  In the following, the '#if'
// preprocessor directive compares 'BBE_VERSION' (i.e., the latest BBE version,
// excluding the patch version) to a specified major and minor version composed
// using the 'BDE_MAKE_VERSION' macro:
//..
//  #if BBE_VERSION > BDE_MAKE_VERSION(1, 3)
//      // Call 'newFunction' for BBE versions later than 1.3.
//      int result = newFunction();
//  #else
//      // Call 'oldFunction' for BBE version 1.3 or earlier.
//      int result = oldFunction();
//  #endif
//..

#ifndef INCLUDED_BDESCM_VERSIONTAG
#include <bdescm_versiontag.h>
#endif

#define BBE_VERSION_MAJOR 2
    // BBE release major version

#define BBE_VERSION_MINOR 7
    // BBE release major version

#define BBE_VERSION BDE_MAKE_VERSION(BBE_VERSION_MAJOR, \
                                     BBE_VERSION_MINOR)
    // Construct a composite version number in the range [ 0 .. 999900 ] from
    // the specified 'BBE_VERSION_MAJOR' and 'BBE_VERSION_MINOR' numbers
    // corresponding to the major and minor version numbers, respectively, of
    // the current (latest) BBE release.  Note that the patch version number is
    // intentionally not included.  For example, 'BBE_VERSION' produces 10300
    // (decimal) for BBE version 1.3.1.

#endif

// ---------------------------------------------------------------------------
// NOTICE:
//      Copyright (C) Bloomberg L.P., 2009
//      All Rights Reserved.
//      Property of Bloomberg L.P. (BLP)
//      This software is made available solely pursuant to the
//      terms of a BLP license agreement which governs its use.
// ----------------------------- END-OF-FILE ---------------------------------
