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

entry::~entry()
{
    tag = -1;
    index = -1;
    offset = -1;
    lru = -1;
    mesi = INVALID;
    raw_address = -1;
}

void entry::evict()
{
    printf("Entry evicted.");
}

int compare_entries(entry to_compare)
{
    if (raw_address != to_compare.get_raw_address())
        return 0;

    return 1;
}

void copy_entry(entry to_copy)
{
    raw_address = to_copy.get_raw_address();
    tag = raw_address;
}
