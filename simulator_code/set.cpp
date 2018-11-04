/* ECE 485/585 Fall Term 2018 Final Project
 * Split L1 Cache simulation, set class functions
 * Members: Patrick Gmerek, Stephen Poanessa, Emma Smith, Amanda Voegtlin
 * 4 November 2018
 */

#include "header.h"

set::set(int set_associativity, int set_index, int set_address_bits, int set_index_bits, int set_offset_bits)
{
    associativity = set_associativity;
    index = set_index;
    address_bits = set_address_bits;
    index_bits = set_index_bits;
    offset_bits = set_offset_bits;
    count = 0;
    
    all_lines = new line[associativity];
    first_line = NULL;
    last_line = NULL;
}

set::~set()
{
    if(lines)
        delete [] lines;
    lines = NULL;
    first_line = NULL;
    last_line = NULL;
}

set::read(unsigned int tag)
{
    result = -1;
    if(count == 0)   // Make sure there's data
    {
        print("miss, no data");
        read_miss_handler();
        result = MISS;
    }
    else
    {
    for(int j = 0; j < associativity; ++j)
            {
                // If line is valid AND line tag matches the desired tag
                if(all_lines[j].mesi != INVALID && tag == lines[j].tag)
                {
                    printf("hit!");
                    update_lru();
                    return HIT;
                }
            }

    
    
}
