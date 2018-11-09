/* ECE 485/585 Fall Term 2018 Final Project
 * Split L1 Cache simulation, set class functions
 * Members: Patrick Gmerek, Stephen Poanessa, Emma Smith, Amanda Voegtlin
 * 4 November 2018
 */

#include "header.h"

set::set(int set_associativity, int set_index, int set_address_bits, int set_index_bits, int set_offset_bits)
{
    associativity = set_associativity;
    index = set_index;
    address_bits = set_address_bits;
    index_bits = set_index_bits;
    offset_bits = set_offset_bits;
    count = 0;
    
    all_tags = new tag_array[associativity];
    first_tag = NULL;
    last_tag = NULL;
}

set::~set()
{
    if(all_tags)
        delete [] all_tags;
    all_tags = NULL;
    first_tag = NULL;
    last_tag = NULL;
}

int set::read(unsigned int tag)
{
    int result = MISS;
    if(count == 0)   // Make sure there's data
        printf("miss, no data");
    else
	    for(int j = 0; j < associativity; ++j)
        {
			// If line is valid AND line tag matches the desired tag
			if(all_tags[j].get_mesi() != INVALID && all_tags[j].get_tag() == tag)     // Tag might not be the right comparator
			{
			    printf("hit!");
			    result = HIT;
                break;
			}
        }

    switch(result)  // Call the appropriate handlers depending on whether there was a read hit or miss
    {
        case MISS:
            read_miss_handler(tag);
            break;
        case HIT:
            update_lru();
            break;
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


