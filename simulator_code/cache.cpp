/* ECE 485/585 Fall Term 2018 Final Project
 * Split L1 Cache simulation, cache class functions
 * Members: Patrick Gmerek, Stephen Poanessa, Emma Smith, Amanda Voegtlin
 * 4 November 2018
*/

#include "header.h"
cache::cache(int assoc)
{
	hits = 0;
	misses = 0;
	reads = 0;
	writes = 0;
	associativity = assoc;
	// Create array of set pointers
	Sets = new set * [NUM_SETS];
	//Set each set pointer to NULL
	for (int i = 0; i < NUM_SETS; ++i)
		Sets[i] = NULL;
}

cache::~cache()
{
    hits = 0;
    misses = 0;
    reads = 0;
    writes = 0;
    associativity = 0;

    if (Sets)   // If Sets exists
    {
        // Delete all sets
        for (int i = 0; i < NUM_SETS; ++i)
        {
            if (Sets[i])    // If the set exists, delete it
            {
                delete Sets[i];
                Sets[i] = NULL;
            }
        }

    delete [] Sets; // Delete the array of Sets
    Sets = NULL;
    }
}

float cache::get_hit_miss_ratio() const
{
    // Convert to float
    float h = hits;
    float m = misses;
    if (!misses)    // Make sure we don't divide by zero
        return 0;
    
    return h / m;   // Returns hits to misses as a float
}

void cache::reset_stats(int verbose)
{	
    hits = 0;
    misses = 0;
    reads = 0;
    writes = 0;

    if (verbose == 2)
	    printf("The cache has been cleared; Hits:%d Misses:%d Reads:%d Writes:%d\n", hits, misses, reads, writes);
}

int cache::invalidate_entry(entry to_invalidate, int verbose)
{
    int set_index = to_invalidate.get_index();
    int success = 0;
    
    if (Sets)
    {
        if (Sets[set_index])   // Set isn't empty
            success = Sets[set_index]->invalidate_snoop(to_invalidate, verbose);
    }

    return success; // Returns either a 0 or 1 for failure or success, respectively
}

int cache::read_request(entry to_add, cache_messages & messages, int verbose) 
{
    int success = 0;
    int current_mesi = get_entry_mesi(to_add, verbose);
    char buffer[BUFFER_SIZE];
    switch (current_mesi)
    {
        case MODIFIED:
            if (verbose == 2)
                printf("Hit\n");
            ++hits;
            success = set_entry_mesi(to_add, SHARED, verbose);
            // Add return data to l2 message to the list of messages 
            sprintf(buffer, "%s%x", RETURN_DATA_TO_L2, to_add.get_raw_address());
            messages.add_message(buffer);
            break;
        case INVALID:
            if (verbose == 2)
                printf("Misses\n");
            ++misses;
            // Mesi stays INVALID
            success = 1;
            break;
        case SHARED:
            if (verbose == 2)
                printf("Hit\n");
            ++hits;
            // Mesi stays SHARED
            success = 1;
            break;
        case EXCLUSIVE:
            if (verbose == 2)
                printf("Hit\n");
            ++hits;
            success = set_entry_mesi(to_add, SHARED, verbose);
            break;
    }
    return success; // Returns either a 0 or 1 for failure or success, respectively
}

int cache::miss_handler(entry to_add, int operation, cache_messages & messages, int verbose)
{
    int success = 0;
    char msg_buffer[BUFFER_SIZE];

    // Send "read from L2" message
    sprintf(msg_buffer, "%s%x", READ_FROM_L2, to_add.get_raw_address());
    messages.add_message(msg_buffer);

    if (operation == 1) // Write miss
    {
        // Send "read for ownership from l2" message
        sprintf(msg_buffer, "%s%x", RFO_FROM_L2, to_add.get_raw_address());
        messages.add_message(msg_buffer);
        success = write_miss_handler(to_add, messages, verbose);
    }
    else    // Read miss
    {
        // Send "read from L2" message
        sprintf(msg_buffer, "%s%x", READ_FROM_L2, to_add.get_raw_address());
        messages.add_message(msg_buffer);
        success = read_miss_handler(to_add, messages, verbose);
    }

    return success; // Returns either a 0 or 1 for failure or success, respectively
}

int cache::read_miss_handler(entry to_add, cache_messages & messages, int verbose)
{
    int success = 0;
    int tag = to_add.get_tag();
    // Check if tag is in other caches (this should never happen)
    if (snoop(tag))
    {
        if (verbose)
            printf("Found entry in other cache, setting mesi to SHARED. This should never happen.\n");
        success = write(to_add, SHARED, messages, verbose);
    }
    else    // Requested is not in other caches, write as exclusive
    {
        success = write(to_add, EXCLUSIVE, messages, verbose);
    }

	return success;
}

int cache::write_miss_handler(entry to_add, cache_messages & messages, int verbose)
{
    return write(to_add, MODIFIED, messages, verbose);
}

// We assume that a snoop to L2 will always be false
// For this project we assume that anything not in L1 is also not in L2
int cache::snoop(unsigned int tag) const
{
	return false;
}

int cache::contains(entry compare_to, int verbose)
{
    int set_index = compare_to.get_index(); // Get the set index
    bool match = 0;
    
    if (Sets && Sets[set_index])   // If Sets exists and set isn't empty
        match = Sets[set_index]->contains(compare_to, verbose); // Check if the entry is within the set
    else
    {
        // Create sets of empty lines
        Sets = new set * [NUM_SETS];
        for (int i = 0; i < NUM_SETS; ++i)
            Sets[i] = NULL;
    }
    return match;
}
  
int cache::write(entry to_add, int new_mesi, cache_messages & messages, int verbose)
{
    int set_index = to_add.get_index();     // Get the set index
    int success = 0; 

    if (Sets)   // If the sets exist....
    {
        if (Sets[set_index])   // Set isn't empty
            success = Sets[set_index]->write(to_add, new_mesi, messages, verbose);
        else    // Set is empty, make a new one
        {
            if (verbose == 2)
                printf("Making new set with %x. and set index %x\n", associativity, set_index);
            Sets[set_index] = new set(associativity);
            success = Sets[set_index]->write(to_add, new_mesi, messages, verbose);
        }
    }
    else    // Otherwise, make it and call write again
    {
        Sets = new set * [NUM_SETS];
        for (int i = 0; i < NUM_SETS; ++i)
            Sets[i] = NULL;
        // Recursive call now that the set exists
        success = write(to_add, new_mesi, messages, verbose);
    }

    return success;
}

// Delete all entries in all sets and all sets in the cache
int cache::clear(int verbose)
{
    reset_stats(verbose);
    if (Sets)   // If Sets exists
    {
        for (int i = 0; i < NUM_SETS; ++i)  // Delete each entry in a set
        {
            if (Sets[i]) // If the set exists, delete it
            {
                delete Sets[i];
                Sets[i] = NULL;
            }
        }
        if (verbose == 2)
            printf("Sets Cleared.\n");

        delete [] Sets; // Delete the entire array
        Sets = NULL;
    }
    return 1;
}

// Gets the mesi for a specific entry
int cache::get_entry_mesi(entry to_retrieve, int verbose) const
{
    int mesi = -1;  // Return ERROR if the entry is not found
    int set_index = to_retrieve.get_index();
    
    if (Sets)   // Make sure Sets exists
    {
        if (Sets[set_index])   // Set isn't empty
            mesi = Sets[set_index]->get_entry_mesi(to_retrieve, verbose);
    }
    return mesi;
}

// Sets the mesi for a specific entry
int cache::set_entry_mesi(entry to_set, int new_mesi, int verbose) 
{
    int success = -1;   // Return ERROR if the entry is not found
    int set_index = to_set.get_index();
    
    if (Sets)   // Make sure Sets exists
    {
        if (Sets[set_index])   // Set isn't empty
            success = Sets[set_index]->set_entry_mesi(to_set, new_mesi, verbose);
    }
    return success;
}

void cache::print_contents() const
{
     if (Sets) // if the cache has data
    {
        for (int i = 0; i < NUM_SETS; ++i)
        {
            if (Sets[i]) //print out the contents of every set
            {
                printf("-------------------------Set %d------------------------\n", i);
                Sets[i]->print_all_entries();
            }
        }
    }
    else
        printf("Cache is empty");
}

void cache::print_statistics (void) const
{
    printf(" Reads: %d\n Writes: %d\n Hits: %d\n Misses: %d\n Hit-Miss Ratio: %.2f\n", 
            reads, writes, hits, misses, get_hit_miss_ratio());
}

