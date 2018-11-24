/* ECE 485/585 Fall Term 2018 Final Project
 * Split L1 Cache simulation, line class functions
 * Members: Patrick Gmerek, Stephen Poanessa, Emma Smith, Amanda Voegtlin
 * 4 November 2018
 */

#include "header.h"

entry::entry()
{
    tag = -1;
    index = -1;
    offset = -1;
    lru = -1;
    mesi = INVALID;
    raw_address = -1;
    empty = 1;
}

entry::entry(entry to_copy, int verbose)
{
    // Raw address contains the tag, set index, and byte offset
    raw_address = to_copy.get_raw_address();
    // Use masks defined in header to find each field
    tag = (raw_address & MASK_FOR_TAG) >> 20;
    index = (raw_address & MASK_FOR_INDEX) >> 6;
    offset = raw_address & MASK_FOR_BYTE_OFFSET;
    empty = 0;
    mesi = INVALID;
    if (verbose == 2)
        printf("Created raw_address %x, tag %d, index %d, and offset %d.\n", raw_address, tag, index, offset);
}

entry::~entry()
{
    tag = -1;
    index = -1;
    offset = -1;
    lru = -1;
    mesi = INVALID;
    raw_address = -1;
    empty = 1;
}

int entry::compare_entries(entry to_compare, int verbose) const
{
    if (verbose == 2)
        printf("Comparing %d to %d.\n", tag, to_compare.get_tag());
    // Only comparing tag so that we get a hit even if the byte
    // offset is different
    if (tag != to_compare.get_tag())
        return 0;

    return 1;
}


// This is is the invalidate for snoop 
int entry::invalidate_snoop(int verbose)
{
    int increment_hits = 0;
    // Invalidating is different for each current mesi state
    switch (mesi)
    {
        case MODIFIED:  // Increment hits and writeback to L2
            mesi = INVALID;
            increment_hits = 1;
            if (verbose)
                printf("%x has been changed from modified to invalid.\n", raw_address);
            //printf("Writing %x back to L2 cache and invalidating entry in L1.\n", raw_address);
            break;
        case INVALID:   // Do nothing
            if (verbose)
                printf("%x has been changed from invalid to invalid.\n", raw_address);
            break;
        case SHARED:    // Invalidate, but don't increment hits or writes
            mesi = INVALID;
            if (verbose)
                printf("%x has been changed from shared to invalid.\n", raw_address);
            break;
        case EXCLUSIVE: // Invalidate, but don't increment hits or writes
            mesi = INVALID;
            if (verbose)
                printf("%x has been changed from exclusive to invalid.\n", raw_address);
            break;
    }

    return increment_hits;
}

void entry::copy_entry(entry to_copy, int verbose)
{
    // Raw address contains the tag, set index, and byte offset
    raw_address = to_copy.get_raw_address();
    // Use masks defined in header to find each field
    tag = (raw_address & MASK_FOR_TAG) >> 20;
    index = (raw_address & MASK_FOR_INDEX) >> 6;
    offset = raw_address & MASK_FOR_BYTE_OFFSET;
    empty = 0;
    mesi = INVALID;
    if (verbose == 2)
        printf("Copied raw_address %x, tag %d, index %d, and offset %d.\n", raw_address, tag, index, offset);
}

void entry::populate_entry(int raw_addr, int verbose)
{
    // Raw address contains the tag, set index, and byte offset
    raw_address = raw_addr;
    // Use masks defined in header to find each field
    tag = (raw_address & MASK_FOR_TAG) >> 20;
    index = (raw_address & MASK_FOR_INDEX) >> 6;
    offset = raw_address & MASK_FOR_BYTE_OFFSET;
    empty = 0;
    mesi = INVALID;
    if (verbose == 2)
        printf("Created tag %d, index %d, and offset %d.\n", tag, index, offset);
}
void entry::dec_lru(int verbose)
{
    if (!empty) // Only decrement if the entry is not empty
    {
        if (verbose == 2)
            printf("Decrementing lru from %d to %d.\n", lru, lru - 1);
        --lru; 
    }
}

void entry::inc_lru(int verbose)
{ 
    if (!empty) // Only decrement if the entry is not empty
    {
        if (verbose == 2)
            printf("Incrementing lru from %d to %d.\n", lru, lru + 1);
        ++lru;
    }
}
