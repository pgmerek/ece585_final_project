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
     /*   for (int k = 0; k < associativity; ++k)
            if (all_tags[k])
                delete all_tags[k];
*/
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
    else
        result = -1;

    return result;
}
  
int set::write(entry to_add, int verbose)
{
    int success = 0;
    int j = 0;
    if (all_tags)
    {
        // Try to find the first empty entry
        while (!all_tags[j].is_empty() && j < associativity)
            ++j;

        if (j == associativity) // Set is full
            evict(to_add, verbose);
        else
        {
            all_tags[j].copy_entry(to_add, verbose);
            update_lru(j);
        }
        success = 1;
    }
    else
        success = -1;

    return success;
}

void set::evict(entry to_add, int verbose)
{
    for(int j = 0; j < associativity; ++j)
    {
        if(all_tags[j].get_lru() == 0)
        {
            all_tags[j].evict();
            all_tags[j].copy_entry(to_add, verbose);
            update_lru(j);
            break;
        }
    }
}
        
void set::update_lru(int entry_index)     // Index is this context is NOT the same set index. This index just tells the function where the new entry is in the set
{
    for (int k = 0; k < associativity; ++k)
    {
        if (k == entry_index)   // Set the new entry to the MRU
        {
            all_tags[k].set_lru(7);
            printf("Setting lru to 7 for new entry.\n");
        }
        else    // Decrement all others
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
