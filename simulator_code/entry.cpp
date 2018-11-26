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
}

entry::entry(entry to_copy, int verbose)
{
    // Raw address contains the tag, set index, and byte offset
    raw_address = to_copy.get_raw_address();
    // Use masks defined in header to find each field
    tag = (raw_address & MASK_FOR_TAG) >> 20;
    index = (raw_address & MASK_FOR_INDEX) >> 6;
    offset = raw_address & MASK_FOR_BYTE_OFFSET;
    mesi = INVALID;
    lru = -1;
    if (verbose == 2)
        printf("Created raw_address %x, tag %x, index %x, and offset %x.\n", raw_address, tag, index, offset);
}

entry::~entry()
{
    tag = -1;
    index = -1;
    offset = -1;
    lru = -1;
    mesi = INVALID;
    raw_address = -1;
}

void entry::dec_lru(int verbose)
{
    if (verbose == 2)
        printf("Decrementing lru of %x from %d to %d.\n", tag, lru, lru - 1);
    --lru; 
}

void entry::copy_entry(entry to_copy, int verbose)
{
    // Raw address contains the tag, set index, and byte offset
    raw_address = to_copy.get_raw_address();
    // Use masks defined in header to find each field
    tag = (raw_address & MASK_FOR_TAG) >> 20;
    index = (raw_address & MASK_FOR_INDEX) >> 6;
    offset = raw_address & MASK_FOR_BYTE_OFFSET;
    lru = -1;
    mesi = INVALID;
    if (verbose == 2)
        printf("Copied raw_address %x, tag %x, index %x, and offset %x.\n", raw_address, tag, index, offset);
}

void entry::populate_entry(int raw_addr, int verbose)
{
    // Raw address contains the tag, set index, and byte offset
    raw_address = raw_addr;
    // Use masks defined in header to find each field
    tag = (raw_address & MASK_FOR_TAG) >> 20;
    index = (raw_address & MASK_FOR_INDEX) >> 6;
    offset = raw_address & MASK_FOR_BYTE_OFFSET;
    mesi = INVALID;
    lru = -1;
    if (verbose == 2)
        printf("Created tag %d, index %d, and offset %d.\n", tag, index, offset);
}

int entry::compare_entries(entry to_compare, int verbose) const
{
    if (verbose == 2)
        printf("Comparing %x to %x.\n", tag, to_compare.get_tag());
    // Only comparing tag so that we get a hit even if the byte
    // offset is different
    if (tag != to_compare.get_tag())
        return 0;

    return 1;
}

// This is is the invalidate for snoop 
int entry::invalidate_snoop(int verbose)
{
    int result = DO_NOTHING;
    // Invalidating is different for each current mesi state
    switch (mesi)
    {
        case MODIFIED:  // Increment hits and writeback to L2
            mesi = INVALID;
            result = HIT;
            if (verbose == 2)
                printf("%x has been changed from modified to invalid.\n", raw_address);
            break;
        case INVALID:   // Increment misses 
            if (verbose == 2)
                printf("%x has been changed from invalid to invalid.\n", raw_address);
            result = MISS;
            break;
        case SHARED:    // Invalidate, but don't increment hits or writes
            mesi = INVALID;
            if (verbose == 2)
                printf("%x has been changed from shared to invalid.\n", raw_address);
            result = DO_NOTHING;
            break;
        case EXCLUSIVE: // Invalidate, but don't increment hits or writes
            mesi = INVALID;
            if (verbose == 2)
                printf("%x has been changed from exclusive to invalid.\n", raw_address);
            result = DO_NOTHING;
            break;
    }

    return result;
}

