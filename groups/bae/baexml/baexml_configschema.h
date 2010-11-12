// baexml_configschema.h   -*-C++-*-   GENERATED FILE -- DO NOT EDIT
#ifndef INCLUDED_BAEXML_CONFIGSCHEMA
#define INCLUDED_BAEXML_CONFIGSCHEMA

#ifndef INCLUDED_BDES_IDENT
#include <bdes_ident.h>
#endif
BDES_IDENT_RCSID(baexml_configschema_h,"$Id$ $CSID$ $CCId$")
BDES_IDENT_PRAGMA_ONCE

//@PURPOSE: Provide the 'baexml' schema
//
//@CLASSES:
// baexml_ConfigSchema: the configuration schema
//
//@AUTHOR: Rohan Bhindwale (rbhindwale@bloomberg.net)
//
//@DESCRIPTION: This component provides a namespace for the
// 'baexml' schema.  This in-core representation of the schema
// is used to validate XML configuration data used to initialize the service.

#ifndef INCLUDED_BAESCM_VERSION
#include <baescm_version.h>
#endif

#ifndef INCLUDED_BSL_OSTREAM
#include <bsl_ostream.h>
#endif

namespace BloombergLP {

struct baexml_ConfigSchema {
    // Provide a namespace for the 'baexml' schema.

    static const char TEXT[];    // 'baexml' schema
};

// FREE OPERATORS
bsl::ostream& operator<<(bsl::ostream& stream, const baexml_ConfigSchema& schema);
    // Write the specified 'schema' to the specified 'stream', and return
    // a modifiable reference to 'stream'.


// FREE OPERATORS
inline
bsl::ostream& operator<<(
    bsl::ostream& stream,
    const baexml_ConfigSchema& schema)
{
    return stream << schema.TEXT;
}

}  // close namespace BloombergLP
#endif

// GENERATED BY BLP_BAS_CODEGEN_3.5.0 Tue May  4 08:22:47 2010
// ----------------------------------------------------------------------------
// NOTICE:
//      Copyright (C) Bloomberg L.P., 2010
//      All Rights Reserved.
//      Property of Bloomberg L.P. (BLP)
//      This software is made available solely pursuant to the
//      terms of a BLP license agreement which governs its use.
// ------------------------------ END-OF-FILE ---------------------------------
