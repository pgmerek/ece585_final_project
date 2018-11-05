/* ECE 485/585 Fall Term 2018 Final Project
 * Split L1 Cache simulation, line class functions
 * Members: Patrick Gmerek, Stephen Poanessa, Emma Smith, Amanda Voegtlin
 * 4 November 2018
 */

#include "header.h"

tag_array::tag_array()
{
    tag = -1;
    lru = -1;
    next = NULL;
    prev = NULL;
    mesi = INVALID;
}

tag_array::~tag_array()
{
    next = NULL;
    prev = NULL;
}

tag_array::set_tag(int new_tag)
{
    tag = new_tag;
    return 1;
}

tag_array::set_lru(int new_lru)
{
    lru = new_lru;
    return 1;
}

tag_array::get_mesi(char * return_string)
{
    switch(mesi)
    {
        case MODIFIED:
            strcpy(return_string, "MODIFIED");
            break;
        case INVALID:
            strcpy(return_string, "INVALID");
            break;
        case SHARED:
            strcpy(return_string, "SHARED");
            break;
        case EXCLUSIVE:
            strcpy(return_string, "EXCLUSIVE");
            break;
    }

    if(!return_string)
            return 0;
    return 1;
}
