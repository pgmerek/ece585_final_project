/* ECE 485/585 Fall Term 2018 Final Project
 * Split L1 Cache simulation, line class functions
 * Members: Patrick Gmerek, Stephen Poanessa, Emma Smith, Amanda Voegtlin
 * 4 November 2018
 */

#include "header.h"
        
traces::traces()
{
    operation = -1;
    address = -1;
    has_address = false;
}

traces::~traces()
{
    operation = -1;
    address = -1 ;
    has_address = false;
}

bool traces::populate(char * line, int verbose)
{
    char * endPtr;
    bool error = true;
    if (line)
    {
        operation = strtol(line, &endPtr, 10);  // Reads the operation from the line
        if (operation >= 8)
        {
            has_address = false;
            address = -1;
            if (verbose == 2)
                printf("Operation: %d, address: (none)\n", operation);
        }
        else
        {
            has_address = true;
            address = strtol(endPtr, NULL, 16);  // Convert hex string address to long integer
            if (verbose == 2)
                printf("Operation: %d, address: %x.\n", operation, address);
        }
        error = false;
    }

    return error;
}

