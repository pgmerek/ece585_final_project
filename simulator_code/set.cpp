/* ECE 485/585 Fall Term 2018 Final Project
 * Split L1 Cache simulation, set class functions
 * Members: Patrick Gmerek, Stephen Poanessa, Emma Smith, Amanda Voegtlin
 * 4 November 2018
 */

#include "header.h"


set::set(int assoc)
{
    associativity = assoc;
    count = 0;
	all_tags = new entry[associativity];
}

set::set(int assoc, entry new_entry, int verbose)
{
    associativity = assoc;
    count = 0;
	all_tags = new entry[associativity];
    all_tags[0].copy_entry(new_entry, verbose);
    update_lru(0);  // Updated lru for the first item
}

set::~set()
{
    associativity = -1;
    count = -1;
    if (all_tags)
    {
        delete [] all_tags;
        all_tags = NULL;
    }
}
int set::contains(entry compare_to)
{
    int result = MISS;
    if (all_tags)
    {
        // Search for a matching entry
        for (int j = 0; j < associativity; ++j)
        {
            if (all_tags[j].compare_entries(compare_to))
            {
                result = HIT;
                update_lru(j);
                break;
            }
        }
    }
    else    // If our entries don't exist, return ERROR
        result = ERROR;

    return result;
}
  
int set::write(entry to_add, int verbose)
{
    int success = 0;
    int j = 0;
    int lru_index = -1; // If the set is full, this is where we should write the new entry
    if (all_tags)
    {
        // Try to find the first empty entry
        while (!all_tags[j].is_empty() && j < associativity)
            ++j;

        if (j == associativity - 1) // Set is full
        {
            printf("Set full in write operation. Evicting the lru.\n");
            lru_index = evict(to_add, verbose);
            update_lru(lru_index);
        }
        else    // Set isn't full
        {
            printf("Set isn't full in write operation.\n");
            all_tags[j].copy_entry(to_add, verbose);
            update_lru(j);
        }
        success = 1;
    }
    else    // If our entries don't exist, return ERROR
        success = ERROR;

    return success;
}

int set::evict(entry to_add, int verbose)
{
    // Search for the lru 
    for(int j = 0; j < associativity; ++j)
    {
        if(all_tags[j].get_lru() == 0)  // Lru is 0, mru is 7
        {
            all_tags[j].evict();    // Evict the lru
            all_tags[j].copy_entry(to_add, verbose);    // Replace with new entry
            return j;
        }
    }
}
        
void set::update_lru(int entry_index)     // Index is this context is NOT the same set index. This index just tells the function where the new entry is in the set
{
    // Retain old lru because we need to decrement the entries that
    // are more recent than the one we replace. Only used if set is full
    int old_lru = all_tags[entry_index].get_lru();
    for (int k = 0; k < associativity; ++k)
    {
        if (k == entry_index)   // Set the new entry to the MRU
        {
            all_tags[k].set_lru(7);
            printf("Setting lru to 7 for new entry.\n");
        }
        // Decrement all other that were newer than the previous entry
        else if (all_tags[k].get_lru() > old_lru)
            all_tags[k].dec_lru();
    }
}

int set::is_full(void) 
{
   for (int j = 0; j < associativity; ++j)
       if (all_tags[j].is_empty())
           return 0;

   return 1;
}

int set::is_empty(void) 
{
   for (int j = 0; j < associativity; ++j)
       if (!all_tags[j].is_empty())
           return 0;

   return 1;
}

