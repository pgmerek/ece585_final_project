/* ECE 485/585 Fall Term 2018 Final Project
 * Header file for main.cpp
 * Members: Patrick Gmerek, Stephen Poanessa, Emma Smith, Amanda Voegtlin
 * 4 November 2018
 */

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <fstream>

using namespace std;

#define NUM_SETS 16384
#define BYTE_LINES 64
#define CPU_BITS 32
// Instruction cache constants
#define INSTR_NUM_LINES 4
// Data cache constants
#define DATA_NUM_LINES 8
// Standard size for character array buffers
#define BUFFER_SIZE 256
#define HIT 1
#define MISS 0
#define ERROR -1
// Define the MESI States
#define MODIFIED 0
#define INVALID 1
#define SHARED 2
#define EXCLUSIVE 3

// Address mask constants
#define MASK_FOR_TAG 0xFFF00000
#define MASK_FOR_INDEX 0x000FFFC0
#define MASK_FOR_BYTE_OFFSET 0x0000003F

// Forward declare all classes
class cache;
class set;
class entry;

// Declare the necessary classes
class cache
{
    public:
        cache(int assoc);
        ~cache();
        // Get functions
        int get_reads() const { return reads; } 
        int get_writes() const { return writes; }
        int get_hits() const { return hits; }
        int get_misses() const { return misses; }
        int reset_stats(int verbose);
        float get_hit_miss_ratio() const;
        // Set functions
        void increment_reads(void) { ++reads; };
        void increment_writes(void) { ++writes; };
        void increment_hits(void) { ++hits; };
        void increment_misses(void) { ++misses; };
        // All others
        int invalidate_snoop(entry invalid_entry, int verbose);
        int miss_handler(entry to_add, int operation, int verbose);
        int shared_memory(entry tag, int operation);
        int shared_snoop(entry tag, int operation);
        int modified_memory(entry tag, int operation);
        int snoop(unsigned int tag) const;
        int contains(entry compare_to, int verbose);
        int write(entry to_add, int verbose);
        int write(entry to_add, int new_mesi, int verbose);
        int clear (int verbose);
        void print_contents(void) const;

    private:
        // Number of...
        int hits;
        int misses;
        int reads;
        int writes;
        // Cache parameters
        int associativity;
        // Pointer to the sets in the cache
        set ** Sets;
        // Private functions
        int read_miss_handler(entry to_add, int verbose);
        int write_miss_handler(entry to_add, int verbose);
};

class set
{
    public:
        set(int assoc, entry new_entry, int verbose);
        set(int assoc, entry new_entry, int new_mesi, int verbose);
        set(int assoc);
        ~set(void);
        // Get functions
        int is_full(void);
        int is_empty(void);
        // Other functions
        int read(unsigned int tag);
        int write(entry to_add, int verbose);
        int write(entry to_add, int new_mesi, int verbose);
        int contains(entry compare_to, int verbose);
        int evict(entry to_add, int verbose);
        int invalidate_snoop(entry to_invalidate, int verbose);
        void update_lru(int index, int verbose);

    private:
        entry * all_tags;   // All lines in the set
        int count;
        int associativity;
};

// Object for tag array/entry/line
class entry
{
    public:
        entry();
        ~entry();
        // Set functions
        void set_tag(int new_tag) { tag = new_tag; };
        void set_index(int new_index) { index = new_index; };
        void set_offset(int new_offset) { offset = new_offset; };
        void set_lru(int new_lru) { lru = new_lru; };
        void dec_lru(int verbose);
        void inc_lru(int verbose);
        void set_mesi(int new_mesi) { mesi = new_mesi; };
        void set_raw_address(int new_raw_address) { raw_address = new_raw_address; };
        // Get functions
        int get_tag(void) const { return tag; }
        int get_index(void) const { return index; }
        int get_offset(void) const { return offset; }
        int get_lru(void) const { return lru; }
        int get_mesi(void) const { return mesi; }
        int get_raw_address(void) const { return raw_address; }
        int is_empty(void) const { return empty; }
        // All others
        void evict(int verbose);
        void copy_entry(entry to_copy, int verbose);
        void populate_entry(int raw_addr, int verbose);
        int compare_entries(entry to_compare, int verbose) const;
        int invalidate_snoop(int verbose);

    private:
        bool empty;
        int tag;
        int index;
        int offset;
        int lru;
        int mesi;
        int raw_address;
};

class traces
{
    public:
        traces(void);
        ~traces(void);
        bool populate(char * line, int verbose);
        int get_operation(void) const { return operation; };
        int get_address(void) const { return address; };
    private:
        int operation;
        bool has_address;   // False for when operation equal 8 or 9, true otherwise
        int address;
};

int read_file(traces ** references, char * fileName, int verbose);
