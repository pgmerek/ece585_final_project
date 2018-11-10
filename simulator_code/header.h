/* ECE 485/585 Fall Term 2018 Final Project
 * Header file for main.cpp
 * Members: Patrick Gmerek, Stephen Poanessa, Emma Smith, Amanda Voegtlin
 * 4 November 2018
 */

#include <cstdio>
#include <cstring>

#define CPU_BITS 32
// Instruction cache constants
#define INSTR_NUM_LINES 4
#define INSTR_NUM_SETS 16384
#define INSTR_BYTE_LINES 64
// Data cache constants
#define DATA_NUM_LINES 8
#define DATA_NUM_SETS 16384
#define DATA_BYTE_LINES 64
// Standard size for character array buffers
#define BUFFER_SIZE 256
#define HIT 1
#define MISS 0
// Define the MESI States
#define MODIFIED 0
#define INVALID 1
#define SHARED 2
#define EXCLUSIVE 3

// Address mask constants
#define MASK_FOR_TAG 0xFFF00000
#define MASK_FOR_SET 0x000FFFC0
#define MASK_FOR_BYTE_INDEX = 0x0000003F

// Forward declare all classes
class cache;
class set;
class tag_array;

// Declare the necessary classes
class cache
{
    public:
        cache();
        ~cache();
        int get_reads() const { return reads; } 
        int get_writes() const { return writes; }
        int get_hits() const { return hits; }
        int get_misses();
        float hit_miss_ratio();

    private:
        // Number of...
        int hits;
        int misses;
        int reads;
        int writes;
        int operations;
        // Cache parameters
        int set_size;
        int associativity;
        // Pointer to the sets in the cache
        set * Sets;
};

class set
{
    public:
        set(int set_associativity, int set_index, int set_address_bits, int set_index_bits, int set_offset_bits);
        ~set();
        int read(unsigned int tag);
        int is_full(void);
	void read_miss_handler(unsigned int tag);
	void update_lru(void);

    private:
        tag_array * all_tags;   // All lines in the set
        tag_array * first_tag;
        tag_array * last_tag;
        int count;
        int associativity;
        unsigned int index;
        unsigned int address_bits;
        unsigned int index_bits;
        unsigned int offset_bits;
        // Private functions
        void read_miss_handler(void);
        

};

// Tag array for each line
class tag_array
{
    public:
        tag_array();
        ~tag_array();
        int set_tag(unsigned int new_tag);
        int set_lru(int new_lru);
        int get_mesi(void) const { return mesi; }
        unsigned int get_tag(void) const { return tag; }
        int get_lru(void) const { return lru; }
	void evict(void);

    private:
        // Doesn't store set or byte offset
        // Set is determined by which set object this tag object is in.
        // Byte offset might not matter since we're just matching lines
        unsigned int tag;
        int lru;
        int mesi;
};

