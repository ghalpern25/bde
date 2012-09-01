// bslalg_hashtableutil.cpp                                           -*-C++-*-
#include <bslalg_hashtableutil.h>

#include <bsls_ident.h>
BSLS_IDENT("$Id$ $CSID$")

#include <bslalg_bidirectionallinklistutil.h>
#include <bslalg_hashtableanchor.h>

#include <bsls_types.h>

#include <stdio.h>

namespace BloombergLP
{

namespace bslalg
{

void HashTableUtil::removeNodeFromTable(HashTableAnchor   *anchor,
                                        BidirectionalLink *position,
                                        size_t             hashCode)
{
    BSLS_ASSERT_SAFE(position);
    BSLS_ASSERT_SAFE(anchor);
    BSLS_ASSERT_SAFE(
                    position->prev() || anchor->listRootAddress() == position);

    // Note that we must update the bucket *before* we unlink from the list,
    // as otherwise we will lose our next/prev pointers.
    HashTableBucket *bucket = bucketForHashCode(*anchor, hashCode);
    if (bucket->first() == position) {
        if (bucket->last() == position) {
            bucket->clear();
        }
        else {
            bucket->setFirst(position->next());
        }
    }
    else if (bucket->last() == position) {
        bucket->setLast(position->prev());
    }

    if (position == anchor->listRootAddress()) {
        anchor->setListRootAddress(position->next());
    }

    BidirectionalLinkListUtil::unlink(position);
}

void HashTableUtil::insertDuplicateAtPosition(HashTableAnchor    *anchor,
                                              BidirectionalLink  *newNode,
                                              size_t              hashCode,
                                              BidirectionalLink  *location)
{
    BSLS_ASSERT_SAFE(anchor);
    BSLS_ASSERT_SAFE(newNode);
    BSLS_ASSERT_SAFE(location);

    BidirectionalListUtil::insertLinkInHead(newNode, location);

    HashTableBucket *bucket = bucketForHashCode(*anchor, hashCode);
    if (location == bucket->first()) {
        bucket->setFirst(newNode);
    }

    if (location == anchor->listRootAddress()) {
        anchor->setListRootAddress(newNode);
    }
}

void HashTableUtil::insertAtFrontOfBucket(HashTableAnchor   *anchor,
                                          BidirectionalLink *newNode,
                                          size_t             hashCode)
{
    BSLS_ASSERT_SAFE(newNode);
    BSLS_ASSERT_SAFE(anchor);

    HashTableBucket *bucket = bucketForHashCode(*anchor, hashCode);
    BSLS_ASSERT_SAFE(bucket);

    if (bucket->first()) {
        BidirectionalListUtil::insertLinkInHead(newNode, bucket->first());
        if (anchor->listRootAddress() == bucket->first()) {
            anchor->setListRootAddress(newNode);
        }
        bucket->setFirst(newNode);
    }
    else {
        // New bucket is required.
       
        BidirectionalListUtil::insertLinkInHead(newNode, 
                                               anchor->listRootAddress());

        // New buckets are inserted in front of the list.
        
        anchor->setListRootAddress(newNode);   
        bucket->setFirstLast(newNode);
    }
}

void HashTableUtil::spliceSegmentIntoBucket(BidirectionalLink  *cursor,
                                            BidirectionalLink  *nextCursor,
                                            HashTableBucket    *bucket,
                                            BidirectionalLink **newRoot)
{
    BSLS_ASSERT_SAFE(cursor);
    BSLS_ASSERT_SAFE(nextCursor);
    BSLS_ASSERT_SAFE(bucket);
    BSLS_ASSERT_SAFE(newRoot);

    // splice the array segment
    if (!bucket->first()) {
        bucket->setFirstLast(cursor, nextCursor);
        BidirectionalListUtil::spliceListBeforeLink(cursor,
                                                    nextCursor,
                                                    *newRoot);
        *newRoot = cursor;
    }
    else {
        BidirectionalLinkListUtil::spliceListBeforeLink(cursor,
                                                        nextCursor,
                                                        bucket->first());
        if (bucket->first() == *newRoot) { // Check before updating 'first'!
            *newRoot = cursor;
        }
        bucket->setFirst(cursor);
    }
}

}  // close namespace BloobmergLP::bslalg
}  // close namespace BloombergLP
// ---------------------------------------------------------------------------
// NOTICE:
//      Copyright (C) Bloomberg L.P., 2012
//      All Rights Reserved.
//      Property of Bloomberg L.P. (BLP)
//      This software is made available solely pursuant to the
//      terms of a BLP license agreement which governs its use.
// ----------------------------- END-OF-FILE ---------------------------------
