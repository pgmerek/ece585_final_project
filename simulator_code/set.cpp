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
int set::contains(entry compare_to) const
{
    int result = MISS;
    printf("Beer");
    if (all_tags)
    {
        // Search for a matching entry
        for (int j = 0; j < associativity; ++j)
        {
            if (all_tags[j].compare_entries(compare_to))
                result = HIT;
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
            all_tags[j].copy_entry(to_add, verbose);
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
            break;
        }
    }
}
        
void set::update_lru()
{
}
int set::is_full(void) 
{
}
