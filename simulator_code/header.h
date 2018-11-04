/* ECE 485/585 Fall Term 2018 Final Project
 * Header file for main.cpp
 * Members: Patrick Gmerek, Stephen Poanessa, Emma Smith, Amanda Voegtlin
 * 4 November 2018
 */

#define CPU_BITS 32
// Instruction cache constants
#define INSTR_NUM_LINES 4
#define INSTR_NUM_SETS 16384
#define INSTR_BYTE_LINES 64
// Data cache constants
#define DATA_NUM_LINES 8
#define DATA_NUM_SETS 16384
#define DATA_BYTE_LINES 64

// Declare an enum for the MESI bits
enum enum_mesi
{
    MODIFIED    = 0;
    INVALID     = 1;
    SHARED      = 2;
    EXCLUSIVE   = 3;
};

// Declare the necessary classes
class cache
{
    public:
        cache();
        ~cache();
        int get_reads() const { return reads} 
        int get_writes() const { return writes };
        int hits();
        int misses();
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
        set ** Sets;
};

class set
{
    pubic:
        set();
        ~set();

    private:

};

class line
{
    pubic:
        line();
        ~line();
        int set_tag(int new_tag);
        int set_lru(int new_lru);
        int get_mesi(void) const;
        int get_tag(void) const { return tag; }
        int get_lru(void) const { return lru; }

    private:
        int tag;
        int lru;
        enum_mesi mesi;
        // Pointers to the next and previous lines relative to this line
        line * next;
        line * prev;
};

