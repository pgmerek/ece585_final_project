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
	Sets = set[NUM_SETS];
	//create sets of empty lines
	for (int i = 0; i < NUM_SETS; ++i)
		Sets[i] = new set(associativity);
	
}
/* Amanda's Section */
// Tranisitions 1 - 3 







/* End of Amanda's Section */

/* Patrick's section */
// Transisition 6 - 7













/* End of Patrick's section */

/* Emma's section */
// Transition 4 - 5

/* This should happen when when the cpu is doing a read and 
 * the data is not in this cache or the L2 cache.
 * Cache reads reads the line in from memory and changes the 
 * line from I to E
 */

int cache::invalid_to_exclusive_memory(tag_array tag)
{
	tag.set_mesi(EXCLUSIVE);



		

}








/* End of Emma's section */
