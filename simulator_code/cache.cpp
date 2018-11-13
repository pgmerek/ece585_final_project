/* ECE 485/585 Fall Term 2018 Final Project
 * Split L1 Cache simulation, cache class functions
 * Members: Patrick Gmerek, Stephen Poanessa, Emma Smith, Amanda Voegtlin
 * 4 November 2018
 */

#include "header.h"
cache::cache(int associativity)
{
	hits = 0;
	misses = 0;
	reads = 0;
	writes = 0;
	operations = 0;
//	Sets = set[NUM_SETS];
	//create sets of empty lines
//	for (int i = 0; i < NUM_SETS; ++i)
//		Sets[i] = new set(associativity);
	
}
/* Amanda's Section */
// Tranisitions: All transitions for lines that that are Modified







/* End of Amanda's Section */

/* Patrick's section */
// Transisition All transitions for lines that are at Exclusive













/* End of Patrick's section */

/* Emma's section */
// Transition handlers for invalid lines
// These were numbers 1, 4, and 7 for the controller accessing memory
// Transition 1 for snoop

// This should be used every time we have a memory request for 
// a line that is invalid 
int cache::invalid_memory(tag_array tag, int operation)
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
int cache::invalid_snoop(tag_array tag)
{
	// This should always happen
	if (snoop(tag.get_tag()) == false)
	    tag.set_mesi(INVALID);
	return 1;
	
}

// Transition handlers for lines that are Shared
int cache::shared_memory(tag_array tag)
{
}

int cache::shared_snoop(tag_array tag)
{
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
