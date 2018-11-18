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

void entry::evict()
{
    printf("Entry evicted.");
    empty = 1;
}

int entry::compare_entries(entry to_compare) const
{
     printf("Comparing %d to %d.\n", tag, to_compare.get_tag());
    if (tag != to_compare.get_tag())
        return 0;

    return 1;
}

void entry::copy_entry(entry to_copy, int verbose)
{
    raw_address = to_copy.get_raw_address();
    tag = (raw_address & MASK_FOR_TAG) >> 20;
    index = (raw_address & MASK_FOR_INDEX) >> 6;
    offset = raw_address & MASK_FOR_BYTE_OFFSET;
    empty = 0;
    if (verbose >= 1)
        printf("Copied raw_address %x, tag %d, index %d, and offset %d.\n", raw_address, tag, index, offset);
}

void entry::populate_entry(int raw_addr, int verbose)
{
    raw_address = raw_addr;
    tag = (raw_address & MASK_FOR_TAG) >> 20;
    index = (raw_address & MASK_FOR_INDEX) >> 6;
    offset = raw_address & MASK_FOR_BYTE_OFFSET;
    empty = 0;
    if (verbose >= 1)
        printf("Created tag %d, index %d, and offset %d.\n", tag, index, offset);
}
void entry::dec_lru(void)
{
    if (!empty)
    {
        printf("Decrementing lru from %d to %d.\n", lru, lru - 1);
        --lru; 
    }
}

void entry::inc_lru(void)
{ 
    if (!empty)
        ++lru;
}
