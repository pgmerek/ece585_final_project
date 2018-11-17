/* ECE 485/585 Fall Term 2018 Final Project
 * Split L1 Cache simulation, line class functions
 * Members: Patrick Gmerek, Stephen Poanessa, Emma Smith, Amanda Voegtlin
 * 4 November 2018
 */

#include "header.h"
        
traces::traces(char * line)
{
    char * endPtr;
    operation = strtol(line, &endPtr, 10);  // Reads the operation from the line
    if (operation >= 8)
        has_address = false;
    else
    {
        has_address = true;
        address = strtol(endPtr, NULL, 16);  // Convert hex string address to long integer
    }
}

traces::~traces();
{
    operation = -1;
    address = 0;
    has_address = false;
}

