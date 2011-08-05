// baetzo_defaultzoneinfocache.h                                      -*-C++-*-
#ifndef INCLUDED_BAETZO_DEFAULTZONEINFOCACHE
#define INCLUDED_BAETZO_DEFAULTZONEINFOCACHE

#ifndef INCLUDED_BDES_IDENT
#include <bdes_ident.h>
#endif
BDES_IDENT("$Id: $")

//@PURPOSE: Provide facilities to manage a default Zoneinfo cache object.
//
//@CLASSES:
//             baetzo_DefaultZoneinfoCache: default Zoneinfo cache utilities
//  baetzo_DefaultZoneinfoCacheScopedGuard: guard for default Zoneinfo cache
//
//@AUTHOR: Stefano Pacifico (spacifico1), Henry Verschell (hverschell)
//
//@SEE_ALSO: baetzo_zoneinfocache, baetzo_timezoneutil
//
//@DESCRIPTION: This component provides a namespace,
// 'baetzo_DefaultZoneinfoCache', for utility functions that install and access
// a default Zoneinfo cache object.  In addition, this component provides a
// mechanism, 'baetzo_DefaultZoneinfoCacheScopedGuard', that facilities the
// temporary installation of the default Zoneinfo cache object (for the
// lifetime of the guard object).
//
// Operations that convert a time value either from, or to, a local time
// representation, commonly require access to time-zone information provided
// from a 'baetzo_ZoneinfoCache' object.  Because a single set of time-zone
// data is generally applicable to an entire task, this component provides the
// installation and use of a process-wide default 'baetzo_ZoneinfoCache' object
// (i.e., a Zoneinfo cache).  See 'baetzo_timezoneutil' for examples of how
// operations may take advantage of this default cache.  Note that the behavior
// is undefined unless this facility is used before 'main' exits.
//
///The Automatically-Installed Default-Cache Instance
///--------------------------------------------------
// If a 'baetzo_ZoneinfoCache' object is not explicitly configured via the
// 'setDefaultCache' method, the *first* call to 'defaultCache' automatically
// initializes and returns a new Zoneinfo cache object (i.e., a default
// default-cache object).  Subsequent calls to the 'defaultCache' method return
// that same object, unless the default Zoneinfo cache object is reset via the
// 'setDefaultCache' method.
//
// Clients are encouraged to use this automatically configured default cache
// object unless there is a particular need to configure it explicitly.
//
///Default Time Zone Data Location
///- - - - - - - - - - - - - - - -
// The mechanism for automatically configuring a default Zoneinfo cache object
// first assumes that the TZ Database files are rooted in a directory
// specified by the 'BDE_ZONEINFO_ROOT_PATH' environment variable.  If
// 'BDE_ZONEINFO_ROOT_PATH' is unset, the 'baetzo_DefaultZoneinfoCache' will
// attempt to use one of a (priority ordered) sequence of several (platform
// specific) standard locations.  Finally, if no standard directory exists that
// contains TZ Database files, the 'baetzo_ZoneinfoCache' will use the current
// working directory.
//
// If a TZ Database file is not available in the "configured" directory,
// requests for that time-zone's data will fail.  Additional information about
// TZ Database files can be found in 'baetzo_datafileloader'.
//
///Thread-Safety
///-------------
// The 'baetzo_DefaultZoneinfoCache::defaultCache' method is *thread-safe*
// unless it is called concurrently with either the 'setDefaultCache' method or
// the 'putenv' POSIX function.  Note that the 'baetzo_ZoneinfoCache' singleton
// is, itself, fully thread-safe (see 'baetzo_zoneinfocache'); it is only the
// installation of the default cache that is not thread-safe.
//
// 'baetzo_DefaultZoneinfoCache::setDefaultCache' is *not* *thread-safe*.  The
// expected usage is that clients that choose to *explicitly* configure the
// default cache for their application (rather than using the automatically
// provided default cache) will set the default cache during the initialization
// of their application (while the application has a single thread).
//
// The 'baetzo_DefaultTimeZoneCacheScopedGuard' class is not, even *minimally*
// *thread-safe* meaning that its constructor cannot be invoked concurrently
// from multiple threads.
//
///Usage
///-----
// The following usage examples demonstrate configuring and accessing the
// default Zoneinfo cache object.
//
///Example 1: Accessing the Default Zoneinfo Cache Object
/// - - - - - - - - - - - - - - - - - - - - - - - - - - -
// A common application of the 'baetzo_DefaultZoneinfoCache' is to determine
// the appropriate time-zone information to use for an operation that accepts a
// Zoneinfo cache as an optional argument (e.g., see 'baetzo_timezoneutil').
// Note that this usage pattern is also seen for the default allocator (see
// 'bslma_default').
//
// First, we declare a function,  'getLocalTimeDescriptor',  that returns the
// local-time descriptor for a given time in a particular time zone.  This
// method takes an optional 'baetzo_ZoneinfoCache' address argument,
// via the default 'zoneinfoCache' parameter.  If 'zoneinfoCache' is
// unspecified or 0, the default Zoneinfo cache is used for the operation:
//..
//  int getLocalTimeDescriptor(baetzo_LocalTimeDescriptor *result,
//                             const bdet_Datetime&        utcTime,
//                             const char                 *timeZoneId,
//                             baetzo_ZoneinfoCache       *zoneinfoCache = 0)
//      // Load, into the specified 'result', the local time descriptor
//      // indicated by the specified 'utcTime' and the specified 'timeZoneId'.
//      // Return 0 on success, and a non-zero value otherwise.  Optionally
//      // specify a 'zoneinfoCache' used to retrieve time-zone information.
//      // If 'zoneinfoCache' is 0, the currently installed default Zoneinfo
//      // cache is used.
//  {
//..
// We call the 'baetzo_DefaultZoneinfoCache::defaultCache' method, which
// returns 'zoneinfoCache' if 'zoneinfoCache' is not 0, and the currently
// installed Zoneinfo cache otherwise.
//..
//      baetzo_ZoneinfoCache *cache =
//                    baetzo_DefaultZoneinfoCache::defaultCache(zoneinfoCache);
//      BSLS_ASSERT(0 != cache);
//..
// Then, now that we have a of Zoneinfo cache object, we access the time-zone
// data for 'timeZoneId', and obtain the local-time descriptor for 'utcTime':
//..
//      const baetzo_Zoneinfo *zoneinfo = cache->getZoneinfo(timeZoneId);
//      if (0 == zoneinfo) {
//
//          // Data for 'timeZoneId' is not available in the cache, so return
//          // an error.
//
//          return 1;                                                 // RETURN
//      }
//..
// Now we invoke the 'findTransitionForUtcTime' method on 'zoneInfo' to obtain
// the transition holding the local time descriptor for the specified
// 'utcTime':
//      baetzo_Zoneinfo::TransitionConstIterator it =
//                                 zoneinfo->findTransitionForUtcTime(utcTime);
//      *result = it->descriptor();
//      return 0;
//  }
//..
// Note that 'findTransitionForUtcTime' has undefined behavior if the supplied
// search time is earlier than that of the first transition in the Zoneinfo
// object.  However a 'baetzo_ZoneinfoCache' (from which we obtained
// 'zoneinfo') is guaranteed to return a well-formed 'baetzo_Zoneinfo' (see
// 'baetzo_ZoneinfoUtil::isWellFormed'), meaning that it contains a transition
// at the first representable 'bdet_Datetime' value, so the following call to
// 'findTransitionForUtcTime' is guaranteed to have defined behavior.
//
//  Finally, we call our 'getLocalDescriptor' defined above:
//..
//  baetzo_LocalTimeDescriptor result;
//  int rc = getLocalTimeDescriptor(&result,
//                                  bdet_Datetime(bdet_Date(2011, 03, 21),
//                                                bdet_Time(20, 57)),
//                                  "America/New_York");
//  assert(0 == rc);
//  assert("EDT"  == result.description());
//  assert(true   == result.dstInEffectFlag());
//  assert(-14400 == result.utcOffsetInSeconds());
//..
//  Notice that since we did not specify the optional 'baetzo_ZoneinfoCache'
//  object, data will automatically come from the configured default
//  'baetzo_ZoneinfoCache' object.
//
///Example 2: Installing a Default Zoneinfo Cache Object
///- - - - - - - - - - - - - - - - - - - - - - - - - - -
// In this example we demonstrate how to configure the default time-zone cache
// for a process.  Note that many application may not need to explicitly
// configure the default cache object, but can instead use the automatically
// configured default object.  Also note that the default time-zone cache is
// intended to be configured by the *owner* of 'main'.  The default object
// should be set during the initialization of an application (while the task
// has a single thread) and unset just prior to termination (when there is
// similarly a single thread):
//..
//  int main(int argc, const char *argv[])
//  {
//
//      // ...
//..
// First, we create and configure a 'baetzo_DataFileLoader' object:
//..
//      baetzo_DataFileLoader loader;
//      loader.configureRootPath("./test");
//..
// Then, we use 'loader' to initialize a 'baetzo_ZoneinfoCache' object:
//..
//      baetzo_ZoneinfoCache cache(&loader);
//..
// Next, we create a 'baetzo_DefaultZoneinfoCacheScopedGuard' to set the
// default Zoneinfo cache for the lifetime of the guard, and then verify the
// the address returned by the 'defaultCache' method is correct:
//..
//      {
//          baetzo_DefaultZoneinfoCacheScopedGuard guard(&cache);
//          assert(&cache == baetzo_DefaultZoneinfoCache::defaultCache());
//
//          // ...
//..
// Finally, at the end of this scope the 'guard' is destroyed, and the default
// Zoneinfo cache is restored to its previous value:
//..
//      }
//      assert(&cache != baetzo_DefaultZoneinfoCache::defaultCache());
//
//      // ...
//  }
//..

#ifndef INCLUDED_BAESCM_VERSION
#include <baescm_version.h>
#endif

#ifndef INCLUDED_BAETZO_ZONEINFOCACHE
#include <baetzo_zoneinfocache.h>
#endif

namespace BloombergLP {

                        // =================================
                        // class baetzo_DefaultZoneinfoCache
                        // =================================

struct baetzo_DefaultZoneinfoCache {
    // This struct provides a namespace for functions that manage and access
    // the default time-zone data cache.

  private:

    // PRIVATE CLASS METHODS
    static baetzo_ZoneinfoCache *instance();
        // Return the address of the currently configured modifiable default
        // time-zone information (Zoneinfo) cache object.

  public:
    // CLASS METHODS
    static baetzo_ZoneinfoCache *defaultCache(baetzo_ZoneinfoCache *cache = 0);
        // Return the default 'baetzo_ZoneinfoCache' object for this process if
        // the specified 'cache' is 0, and 'cache' otherwise.  If 'cache' is 0,
        // and the default Zoneinfo cache object has not been set -- either
        // explicitly with a call to 'setDefaultCache', or implicitly through a
        // prior call to this method -- then initialize a new
        // 'baetzo_ZoneinfoCache' object, set the default cache object to the
        // newly created object, and return its address.

    static const char *defaultZoneinfoDataLocation();
        // Return a null terminated (C-style) string describing the default
        // file-system path of the (expected) root of the Zoneinfo Database
        // time-zone information files.  The returned string will be the
        // current value of the 'BDE_ZONEINFO_ROOT_PATH' environment variable,
        // if set, or else one of a (priority ordered) sequence of
        // (platform-specific) standard directories, if one of those
        // directories exists and contains a sub-set of expected files, and the
        // current directory, otherwise.  The behavior is undefined if the
        // returned address is dereferenced after a subsequent call to the
        // 'putenv' POSIX function to set the 'BDE_ZONEINFO_ROOT_PATH'
        // environment variable.

    static void loadDefaultZoneinfoDataLocations(
                                         bsl::vector<const char *> *locations);
        // Load into the specified 'locations',  the sequence of null
        // terminated C-style strings that characterizes the default paths for
        // Zoneinfo data, accessed by this class, on this platform.

    static baetzo_ZoneinfoCache *setDefaultCache(baetzo_ZoneinfoCache *cache);
        // Set the address of the default 'baetzo_ZoneinfoCache' object to the
        // specified 'cache'.  Return the address of the default cache object
        // that was set by a previous call to this method, or 0 if no call to
        // this method was previously executed.  The behavior is undefined
        // unless (1) 'cache' remains valid until the last operation is
        // performed on this process's default cache, or a subsequent call
        // to 'setDefaultCache', and (2) this method is *not* called from one
        // thread while another thread is attempting to access the default time
        // zone cache instance (i.e., this method is *not* thread-safe).  Note
        // that this method is intended for use *only* by the *owner* of 'main'
        // (and for testing purposes) where the caller affirmatively takes
        // responsibility for the behavior of all clients of the default time
        // zone cache.
};

                 // ============================================
                 // class baetzo_DefaultZoneinfoCacheScopedGuard
                 // ============================================

class baetzo_DefaultZoneinfoCacheScopedGuard {
    // Upon construction, an object of this class saves the current default
    // Zoneinfo cache and installs the user-specified Zoneinfo cache as the
    // default.  On destruction, the previous default Zoneinfo cache is
    // restored.  This class is not even *minimally* *thread* *safe*.
    // For terminology see 'bsldoc_glossary'.

    // DATA
    baetzo_ZoneinfoCache *d_previousCache_p;  // original default Zoneinfo
                                              // cache (to be restored on
                                              // destruction)
  private:
    // NOT IMPLEMENTED
    baetzo_DefaultZoneinfoCacheScopedGuard(
                                const baetzo_DefaultZoneinfoCacheScopedGuard&);
    baetzo_DefaultZoneinfoCacheScopedGuard& operator=(
                                const baetzo_DefaultZoneinfoCacheScopedGuard&);

  public:
    // CREATORS
    explicit baetzo_DefaultZoneinfoCacheScopedGuard(
                                                  baetzo_ZoneinfoCache *cache);
        // Create a scoped guard that installs the specified 'cache' as the
        // default Zoneinfo cache.  Note that the pre-existing default time
        // Zoneinfo cache is automatically restored on destruction of this
        // object.

    ~baetzo_DefaultZoneinfoCacheScopedGuard();
        // Restore the default Zoneinfo cache that was in place when this
        // scoped guard was created, and destroy this guard.
};

// ============================================================================
//                      INLINE FUNCTION DEFINITIONS
// ============================================================================

                        // --------------------------
                        // class baetzo_ZoneinfoCache
                        // --------------------------

// CLASS METHODS
inline
baetzo_ZoneinfoCache *baetzo_DefaultZoneinfoCache::defaultCache(
                                                   baetzo_ZoneinfoCache *cache)
{
    return cache ? cache : instance();
}

                        // --------------------------------------------
                        // class baetzo_DefaultZoneinfoCacheScopedGuard
                        // --------------------------------------------

// CREATORS
inline
baetzo_DefaultZoneinfoCacheScopedGuard::baetzo_DefaultZoneinfoCacheScopedGuard(
                                                   baetzo_ZoneinfoCache *cache)
: d_previousCache_p(baetzo_DefaultZoneinfoCache::setDefaultCache(cache))
{
}

inline
baetzo_DefaultZoneinfoCacheScopedGuard::
~baetzo_DefaultZoneinfoCacheScopedGuard()
{
    baetzo_DefaultZoneinfoCache::setDefaultCache(d_previousCache_p);
}

}  // close namespace BloombergLP

#endif

// ---------------------------------------------------------------------------
// NOTICE:
//      Copyright (C) Bloomberg L.P., 2011
//      All Rights Reserved.
//      Property of Bloomberg L.P.  (BLP)
//      This software is made available solely pursuant to the
//      terms of a BLP license agreement which governs its use.
// ----------------------------- END-OF-FILE ---------------------------------
