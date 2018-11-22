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
	operations = 0;
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
    operations = 0;
    associativity = 0;
    // Delete each set
	for (int i = 0; i < NUM_SETS; ++i)
    {
        if (Sets[i])
        {
            delete Sets[i];
            Sets[i] = NULL;
        }
    }
    // Delete the array of sets
    if (Sets)
    {
        delete [] Sets;
        Sets = NULL;
    }
}

int cache::contains(entry compare_to)
{
    int set_index = compare_to.get_index();
    bool match = 0;
    
    if (Sets)
    {

        if (Sets[set_index])   // Set isn't empty
            match = Sets[set_index]->contains(compare_to);

        if (!match)
            ++misses;
        else
            ++hits;
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
   
int cache::write(entry to_add, int verbose)
{
    int set_index = to_add.get_index();
    int success = 0; 

    if (Sets[set_index])   // Set isn't empty
        success = Sets[set_index]->write(to_add, verbose);
    else    // Set is empty, make a new one
        Sets[set_index] = new set(associativity, to_add, verbose);

    return success;
}

     
/* Amanda's Section */
// Tranisitions: All transitions for lines that that are Modified







/* End of Amanda's Section */

/* Patrick's section */
// Transisition All transitions for lines that are at Exclusive













/* End of Patrick's section */

/* Emma's section */
int cache::clear(int verbose)
{
    reset_stats(verbose);
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
        printf("sets cleared");
    // Delete the array of sets
    if (Sets)
    {
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
    operations = 0;

    if (verbose == 2)
	    printf("The cache has been cleared.\nHits:%d\nMisses:%d\nReads:%d\nWrites:%d\n", hits, misses, reads, writes);
    return 1;
}


// Transition handlers for invalid lines
// These were numbers 1, 4, and 7 for the controller accessing memory
// Transition 1 for snoop

// This should be used every time we have a memory request for 
// a line that is invalid 
int cache::invalid_memory(entry tag, int operation)
{
	// read
	if (operation == 0)
	{
		
		// This should never happen
		if (snoop(tag.get_tag())== true)
			tag.set_mesi(SHARED);
		else 
			// should have message that says "reading from memory..."
			tag.set_mesi(EXCLUSIVE);
	}
	//write
	else if (operation == 1)
	{
		//should have message that says "RFO L2 <address>
		tag.set_mesi(MODIFIED);
	}

	return 1;
}

// Every time the processor snoops the L2 cache for a line 
// that is invalid in L1, the line should remain invalid
int cache::invalid_snoop(entry tag)
{
	// This should always happen
	if (snoop(tag.get_tag()) == false)
	    tag.set_mesi(INVALID);
	return 1;
	
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
int cache::snoop(unsigned int tag)
{
	return false;
}






/* End of Emma's section */
