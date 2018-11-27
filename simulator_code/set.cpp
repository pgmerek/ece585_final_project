/* ECE 485/585 Fall Term 2018 Final Project
 * Split L1 Cache simulation, set class functions
 * Members: Patrick Gmerek, Stephen Poanessa, Emma Smith, Amanda Voegtlin
 * 4 November 2018
 */

#include "header.h"


set::set(int assoc)
{
    associativity = assoc;
	all_tags = new entry * [associativity];
    for (int j = 0; j < associativity; ++j)
        all_tags[j] = NULL;
}

set::set(int assoc, entry new_entry, int new_mesi, int verbose)
{
    associativity = assoc;
	all_tags = new entry * [associativity];
    for (int j = 0; j < associativity; ++j)
        all_tags[j] = NULL;

    // Add entry as the first entry
    all_tags[0] = new entry(new_entry, verbose);
    all_tags[0]->set_mesi(new_mesi);
    update_lru(0, verbose);  // Update lru for the first item
}

set::~set()
{
    if (all_tags)   // If the set exists
    {
        for (int j = 0; j < associativity; ++j) // Delete every entry
        {
            if (all_tags[j])
            {
                delete all_tags[j];
                all_tags[j] = NULL;
            }
        }

        delete [] all_tags; // Delete the array of entries
        all_tags = NULL;
    }
}

int set::set_entry_mesi(entry to_set, int new_mesi, int verbose)
{
    int success = 0;
    if (all_tags)
    {
        // Search for a matching entry
        for (int j = 0; j < associativity; ++j)
        {
            if (all_tags[j] && all_tags[j]->compare_entries(to_set, verbose))   // If entry exists and the entry matches
            {
                all_tags[j]->set_mesi(new_mesi);    // Set mesi to matching entry
                success = 1;    // Return success
                break;
            }
        }
    }

    return success;
}

int set::get_entry_mesi(entry to_retrieve, int verbose) const
{
    int mesi = ERROR;  // If mesi cannot be set because there's no matching entry, return ERROR
    if (all_tags)   // Check if entries exist
    {
        // Search for a matching entry
        for (int j = 0; j < associativity; ++j)
        {
            if (all_tags[j] && all_tags[j]->compare_entries(to_retrieve, verbose))  // If entry exists and the entry matches
            {
                mesi = all_tags[j]->get_mesi(); // Retrieve the mesi and capture it as the return value
                break;
            }
        }
    }

    return mesi;
}

int set::write(entry to_add, int new_mesi, cache_messages & messages, int verbose)
{
    int result = 0;
    int index_to_insert = 0;    // This is where the new entry will be added
    int lru_index = 0; // If the set is full, this is where we should write the new entry
    // Retain old lru because we need to decrement the entries that
    // are more recent than the one we replace. Only used if set is full
    int old_lru = 0;
    bool invalid_entry_evicted = false;
    char buffer[BUFFER_SIZE];

    if (all_tags)   // If the set exists (is not empty)
    {
        // Try to find the first empty entry
        while (index_to_insert < associativity && all_tags[index_to_insert])
            ++index_to_insert;

        if (index_to_insert == associativity) // Set is full
        {
            lru_index = evict();    // Find which entry to evict
            if (verbose == 2)
                printf("LRU that was evicted is %d\n", lru_index);
            if (lru_index != ERROR) // If the lru index was retrieved correctly
            {
                if (all_tags[lru_index])    // Delete the lru
                {
                    old_lru = all_tags[lru_index]->get_lru();
                    if (old_lru != 0)
                        invalid_entry_evicted = true;

                    if (all_tags[lru_index]->get_mesi() == MODIFIED)    // Write back to L2 if entry that is evicted is MODIFIED
                    {
                        sprintf(buffer, "%s%x", WRITE_TO_L2, all_tags[lru_index]->get_raw_address());
                        messages.add_message(buffer);
                    }
                    delete all_tags[lru_index];     // Delete (evict) the entry
                    all_tags[lru_index] = NULL;
                }
                index_to_insert = lru_index;    // We need to write to where the evicted entry used to be, this entry is now empty
            }
            else    // If this message appears, the lru of each entry isn't being decremented and/or set properly
                printf("Was not able to find lru in when attempting to evict entry. The lru was likely not set correctly.\n");
        }
        else    // Set is empty
        {
            if (verbose == 2)
                printf("Set isn't full. No eviction need. Writing %x.\n", to_add.get_raw_address());
        }

        // Add new entry if there was no error
        if (lru_index != ERROR)
        {
            all_tags[index_to_insert] = new entry(to_add, verbose);
            all_tags[index_to_insert]->set_mesi(new_mesi);
            if (invalid_entry_evicted)
                update_lru(index_to_insert, old_lru, verbose);
            else
                update_lru(index_to_insert, verbose);
            result = 1;
        }
    }

    return result;
}

int set::contains(entry compare_to, int operation, int verbose)
{
    int result = MISS;
    if (all_tags)   // If the set exists
    {
        // Search for a matching entry
        for (int j = 0; j < associativity; ++j)
        {
            if (all_tags[j] && all_tags[j]->compare_entries(compare_to, verbose))   // If entry exists and matches
            {
                if (all_tags[j]->get_mesi() == INVALID) // If the entry is already in the cache but INVALID
                {
                    if (operation == 1) // Operation is a write
                    {
                        all_tags[j]->set_mesi(MODIFIED);    // Set it to MODIFIED
                        result = HIT;
                    }
                    else    // Operation is a write
                    {
                        all_tags[j]->set_mesi(EXCLUSIVE);    // Set it to EXCLUSIVE 
                        result = MISS;
                    }
                }
                else
                    result = HIT;
                update_lru(j, verbose);     // Update lru of the matching entry since we used it
                break;
            }
        }
    }

    return result;
}

int set::evict()
{
    // This is where the actual lru is. If we don't find an invalid entry, return this
    int least_recently_used_entry = ERROR; 
    int invalid_entry = -1;  // This is where any invalid entry is, if it exists

    // Search for the lru 
    for(int j = 0; j < associativity; ++j)
    {
        if (all_tags[j])
        {
            if (all_tags[j]->get_lru() == 0)  // Lru is 0, mru is 7
                least_recently_used_entry = j; // Capture the index within the set of the least recently used entry 
                
            if (all_tags[j]->get_mesi() == INVALID)
                invalid_entry = j;
        }
    }

    if (invalid_entry != -1)
        return invalid_entry;

    return least_recently_used_entry;
}
 
int set::invalidate_snoop(entry to_invalidate, int verbose)
{
    int result = ERROR; // If it doesn't find anything to invalidate, return ERROR
    if (all_tags)
    {
        // Search for a matching entry
        for (int j = 0; j < associativity; ++j)
        {
            if (all_tags[j] && all_tags[j]->compare_entries(to_invalidate, verbose))    // If entry exists and the entry matches
            {
                result = all_tags[j]->invalidate_snoop(verbose);    // Invalidate the entry
                update_lru(j, verbose);     // Updated lru
                break;
            }
        }
    }
    return result;
}       


void set::update_lru(int entry_index, int old_lru, int verbose)     // Index is this context is NOT the same set index. This index just tells the function where the new entry is in the set
{
    for (int k = 0; k < associativity; ++k)
    {
        if (k == entry_index)   // Set the new entry to the MRU
        {
            all_tags[k]->set_lru(associativity - 1);
            if (verbose == 2)
                printf("Setting lru of %x to %d.\n", all_tags[entry_index]->get_tag(), associativity - 1);
        }
        // Decrement all other that were newer than the previous entry
        else if (all_tags[k] && all_tags[k]->get_lru() > old_lru)
            all_tags[k]->dec_lru(verbose);
    }
}

void set::update_lru(int entry_index, int verbose)     // Index is this context is NOT the same set index. This index just tells the function where the new entry is in the set
{
    // Retain old lru because we need to decrement the entries that
    // are more recent than the one we replace. Only used if set is full
    int old_lru = all_tags[entry_index]->get_lru();
    if (verbose == 2)
        printf("Old lru is %d", old_lru);
    for (int k = 0; k < associativity; ++k)
    {
        if (k == entry_index)   // Set the new entry to the MRU
        {
            all_tags[k]->set_lru(associativity - 1);
            if (verbose == 2)
                printf("Setting lru of %x to %d.\n", all_tags[entry_index]->get_tag(), associativity - 1);
        }
        // Decrement all other that were newer than the previous entry
        else if (all_tags[k] && all_tags[k]->get_lru() > old_lru)
            all_tags[k]->dec_lru(verbose);
    }
}

void set::print_all_entries(void) const
{
    if (all_tags)
    {
        print_all_tags();
        print_all_mesi();
        print_all_lru();
    }
}

void set::print_all_tags() const
{
    printf("Tag:  ");
    for (int j = 0; j < associativity; ++j)
    {
        if (all_tags[j])
        {
            if (all_tags[j]->get_tag() < 16)
                printf("| %x         ", all_tags[j]->get_tag());
            else if (all_tags[j]->get_tag() < 256 && all_tags[j]->get_tag()> 15)
                printf("| %x        ", all_tags[j]->get_tag());
            else if (all_tags[j]->get_tag() < 4096&& all_tags[j]->get_tag() > 255)
                printf("| %x       ", all_tags[j]->get_tag());
            else //This should never happen
                printf( "Error: Tag is impossibly large");
        }
    }
    printf("|\n");
 
}

void set::print_all_mesi() const
{
    printf("MESI: ");

    for (int j = 0; j < associativity; ++j)
    {
        if (all_tags[j])
        {
            switch(all_tags[j]->get_mesi())
            {
                case MODIFIED: 
                    printf("| Modified  ");
                    break;
                case EXCLUSIVE:
                    printf("| Exclusive ");
                    break;
                case SHARED:
                    printf("| Shared    ");
                    break;
                case INVALID:
                    printf("| Invalid   ");
                    break;
                default: //should never happen
                    printf("Error: No Mesi bits");
            }
        }
    }
    printf("|\n");
}

void set::print_all_lru (void) const
{
    printf("LRU:  ");
    for (int j = 0; j < associativity; ++j)
    {
        if (all_tags[j])
            printf("| %d         ", all_tags[j]->get_lru());
    }
    printf("|\n");
}

