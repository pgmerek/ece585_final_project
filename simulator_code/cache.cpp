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
	//create sets of empty lines
	Sets = new set * [NUM_SETS];
	//Set each set to NULL
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
    // Delete the array of sets
    if (Sets)
    {
    // Delete each set
        for (int i = 0; i < NUM_SETS; ++i)
        {
            if (Sets[i])
            {
                delete Sets[i];
                Sets[i] = NULL;
            }
        }
    delete [] Sets;
    Sets = NULL;
    }
}

int cache::contains(entry compare_to, int verbose)
{
    int set_index = compare_to.get_index();
    bool match = 0;
    
    if (Sets)
    {

        if (Sets[set_index])   // Set isn't empty
            match = Sets[set_index]->contains(compare_to, verbose);
    }
    else
    {
        //create sets of empty lines
        Sets = new set * [NUM_SETS];
        //Set each set to NULL
        for (int i = 0; i < NUM_SETS; ++i)
            Sets[i] = NULL;
    }
    return match;
}
  
int cache::write(entry to_add, int new_mesi, int verbose)
{
    int set_index = to_add.get_index();
    int success = 0; 

    if (Sets)   // If the sets exist....
    {
        if (Sets[set_index])   // Set isn't empty
            success = Sets[set_index]->write(to_add, new_mesi, verbose);
        else    // Set is empty, make a new one
        {
            printf("Making new set with %d. and set index %d\n", associativity, set_index);
            Sets[set_index] = new set(associativity);
            success = Sets[set_index]->write(to_add, new_mesi, verbose);
        }
    }
    else    // Otherwise make it
    {
        Sets = new set * [NUM_SETS];
        //Set each set to NULL
        for (int i = 0; i < NUM_SETS; ++i)
            Sets[i] = NULL;
        // Recursive call 
        success = write(to_add, new_mesi, verbose);
    }

    return success;
}

int cache::clear(int verbose)
{
    reset_stats(verbose);
    // Delete the array of sets
    if (Sets)
    {
    // Delete each set
        for (int i = 0; i < NUM_SETS; ++i)
        {
            if (Sets[i])
            {
                delete Sets[i];
                Sets[i] = NULL;
            }
        }
        if (verbose == 2)
            printf("Sets Cleared.\n");

        delete [] Sets;
        Sets = NULL;
    }
    return 1;
}

int cache::reset_stats(int verbose)
{	
    hits = 0;
    misses = 0;
    reads = 0;
    writes = 0;

    if (verbose == 2)
	    printf("The cache has been cleared; Hits:%d Misses:%d Reads:%d Writes:%d\n", hits, misses, reads, writes);
    return 1;
}

int cache::invalidate_snoop(entry to_invalidate, int verbose)
{
    int set_index = to_invalidate.get_index();
    int success = ERROR;
    
    if (Sets)
    {
        if (Sets[set_index])   // Set isn't empty
            success = Sets[set_index]->invalidate_snoop(to_invalidate, verbose);
    }

    return success;
}

int cache::miss_handler(entry to_add, int operation, int verbose)
{
    int success = 0;
    switch (operation)
    {
        case 0: // Data read
            success = read_miss_handler(to_add, verbose);
            break;
        case 1: // Write
            success = write_miss_handler(to_add, verbose);
            break;
        case 2: // Instruction read 
            success = read_miss_handler(to_add, verbose);
            break;
    }

    return success;
}

int cache::read_miss_handler(entry to_add, int verbose)
{
    int success = 0;
    int tag = to_add.get_tag();
    // Check if tag is in other caches (this should never happen)
    if (snoop(tag))
    {
        if (verbose)
            printf("Found entry in other cache, setting mesi to SHARED. This should never happen.\n");
        to_add.set_mesi(SHARED);
        success = 1;
    }
    else    // Requested is not in other caches, write as exclusive
        success = write(to_add, EXCLUSIVE, verbose);

	return success;
}

int cache::write_miss_handler(entry to_add, int verbose)
{
    return write(to_add, MODIFIED, verbose);
}

// Transition handlers for lines that are Shared
int cache::shared_memory(entry tag, int operation)
{
	// read
	if (operation == 0)
		tag.set_tag(SHARED);
	// write
	// also need to send a write command to the L2 cache
	else if (operation == 1)
		tag.set_tag(MODIFIED);
	return 1;

}

int cache::shared_snoop(entry tag, int operation)
{
	// if L2 cache said to invalidate this line
	if (operation == 3)
		tag.set_tag(INVALID);
	// if L2 is reading a line that is already shared
	else if (operation == 4)
		tag.set_tag(SHARED);
	return 1;
}
// Every time we snoop to L2, it will always 
// come back false because for this project
// we will assume that anything that is not in the L1 
// cache is not in the L2 cache
// We will execute this every time we have a miss in the 
// L1 cache
int cache::snoop(unsigned int tag) const
{
	return false;
}

void cache::print_contents() const
{
    printf("Currently, of the %d operations that have occured, %d have been hits and %d have been misses.\n", hits + misses, hits, misses);
    printf("This represents a hit-miss ratio of %f.\n", get_hit_miss_ratio());
}

float cache::get_hit_miss_ratio() const
{
    // Convert to float
    float h = hits;
    float m = misses;
    if (!misses)    // Make sure we don't divide by zero
        return 0;
    
    return h / m;
}

