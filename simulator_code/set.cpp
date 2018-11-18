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
    if (all_tags)
    {
        for (int j = 0; j < associativity; ++j)
        {
            if (all_tags[j].compare_entries(compare_to))
                result = HIT;
        }
    }
        
    return result;
}
   
void set::read_miss_handler(unsigned int tag)
{
    if(is_full())   // Evict set if the set is full and is LRU
    {
        for(int j = 0; j < associativity; ++j)
        {
            if(all_tags[j].get_lru() == 0)
            {
                all_tags[j].evict();
                all_tags[j].set_tag(tag);
                break;
            }
        }
    }
}
        
void set::update_lru()
{
}
int set::is_full(void) 
{
}
