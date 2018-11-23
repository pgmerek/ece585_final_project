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
	all_tags = new entry * [associativity];
    for (int j = 0; j < associativity; ++j)
        all_tags[j] = NULL;
}

set::set(int assoc, entry new_entry, int verbose)
{
    associativity = assoc;
    count = 0;
	all_tags = new entry * [associativity];
    for (int j = 0; j < associativity; ++j)
        all_tags[j] = NULL;

    all_tags[0] = new entry(new_entry, verbose);
    update_lru(0, verbose);  // Updated lru for the first item
}

set::set(int assoc, entry new_entry, int new_mesi, int verbose)
{
    associativity = assoc;
    count = 0;
	all_tags = new entry * [associativity];
    for (int j = 0; j < associativity; ++j)
        all_tags[j] = NULL;

    all_tags[0] = new entry(new_entry, verbose);
    all_tags[0]->set_mesi(new_mesi);
    update_lru(0, verbose);  // Updated lru for the first item
}

set::~set()
{
    if (all_tags)
    {
        for (int j = 0; j < associativity; ++j)
        {
            if (all_tags[j])
            {
                delete all_tags[j];
                all_tags[j] = NULL;
            }
        }

        delete [] all_tags;
        all_tags = NULL;
    }
}

int set::contains(entry compare_to, int verbose)
{
    int result = MISS;
    if (all_tags)
    {
        // Search for a matching entry
        for (int j = 0; j < associativity; ++j)
        {
            if (all_tags[j] && all_tags[j]->compare_entries(compare_to, verbose))
            {
                result = HIT;
                update_lru(j, verbose);
                break;
            }
        }
    }

    return result;
}
  
int set::write(entry to_add, int verbose)
{
    int success = 0;
    int index_to_insert = 0;
    int lru_index = -1; // If the set is full, this is where we should write the new entry
    if (all_tags)
    {
        // Try to find the first empty entry
        while (all_tags[index_to_insert] && index_to_insert < associativity)
            ++index_to_insert;

        if (index_to_insert == associativity) // Set is full
        {
            if (verbose == 2)
                printf("Set full. Evicting the lru and writing %x.\n", to_add.get_raw_address());
            lru_index = evict();    // Find which entry to evict
            if (all_tags[lru_index])    // Delete the lru
            {
                delete all_tags[lru_index];
                all_tags[lru_index] = NULL;
            }
            index_to_insert = lru_index;    // We need to write location of the entry we evicted
        }
        else
        {
            if (verbose == 2)
                printf("Set isn't full. No eviction need. Writing %x.\n", to_add.get_raw_address());
        }

        all_tags[index_to_insert] = new entry(to_add, verbose);
        update_lru(index_to_insert, verbose);
        success = 1;
    }

    return success;
}

int set::write(entry to_add, int new_mesi, int verbose)
{
    int success = 0;
    int index_to_insert = 0;
    int lru_index = 0; // If the set is full, this is where we should write the new entry
    if (all_tags)
    {
        // Try to find the first empty entry
        while (all_tags[index_to_insert] && index_to_insert < associativity)
            ++index_to_insert;

        if (index_to_insert == associativity) // Set is full
        {
            if (verbose == 2)
                printf("Set full. Evicting the lru and writing %x.\n", to_add.get_raw_address());
            lru_index = evict();    // Find which entry to evict
            if (lru_index != ERROR)
            {
                if (all_tags[lru_index])    // Delete the lru
                {
                    delete all_tags[lru_index];
                    all_tags[lru_index] = NULL;
                }
                index_to_insert = lru_index;    // We need to write location of the entry we evicted
            }
            else
                printf("Was not able to find lru in when attempting to evict entry. The lru was likely not set correctly.\n");
        }
        else
        {
            if (verbose == 2)
                printf("Set isn't full. No eviction need. Writing %x.\n", to_add.get_raw_address());
        }

        all_tags[index_to_insert] = new entry(to_add, verbose);
        all_tags[index_to_insert]->set_mesi(new_mesi);
        update_lru(index_to_insert, verbose);
        if (lru_index != ERROR)
            success = 1;
    }

    return success;
}

int set::evict()
{
    // Search for the lru 
    for(int j = 0; j < associativity; ++j)
        if(all_tags[j] && all_tags[j]->get_lru() == 0)  // Lru is 0, mru is 7
            return j;
    // If not found, return error 
    return ERROR;
}
        
void set::update_lru(int entry_index, int verbose)     // Index is this context is NOT the same set index. This index just tells the function where the new entry is in the set
{
    // Retain old lru because we need to decrement the entries that
    // are more recent than the one we replace. Only used if set is full
    int old_lru = all_tags[entry_index]->get_lru();
    for (int k = 0; k < associativity; ++k)
    {
        if (k == entry_index)   // Set the new entry to the MRU
        {
            all_tags[k]->set_lru(7);
            if (verbose == 2)
                printf("Setting lru of %x to 7.\n", all_tags[entry_index]->get_raw_address());
        }
        // Decrement all other that were newer than the previous entry
        else
        {
            if (all_tags[k] && (all_tags[k]->get_lru() > old_lru))
                all_tags[k]->dec_lru(verbose);
        }
    }
}

int set::is_full(void) 
{
   for (int j = 0; j < associativity; ++j)
       if (all_tags[j]->is_empty())
           return 0;

   return 1;
}

int set::is_empty(void) 
{
   for (int j = 0; j < associativity; ++j)
       if (!all_tags[j]->is_empty())
           return 0;

   return 1;
}

int set::invalidate_snoop(entry to_invalidate, int verbose)
{
    int result = ERROR;
    if (all_tags)
    {
        // Search for a matching entry
        for (int j = 0; j < associativity; ++j)
        {
            if (all_tags[j] && all_tags[j]->compare_entries(to_invalidate, verbose))
            {
                result = all_tags[j]->invalidate_snoop(verbose);
                update_lru(j, verbose);
                break;
            }
        }
    }

    return result;
}

