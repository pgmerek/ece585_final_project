/* ECE 485/585 Fall Term 2018 Final Project
 * Split L1 Cache simulation
 * Members: Patrick Gmerek, Stephen Poanessa, Emma Smith, Amanda Voegtlin
 * 4 November 2018
 */

#include "header.h"

//Arg = mode (0 =summary of staticscs, 1= verbose)
int main(int argc, char * argv[])
{
    // Read in the file before doing anything else
    char fileName[BUFFER_SIZE];
    bool verbose = false;
    if (argc != 2)
    {
        strcpy(fileName, argv[1]);
        verbose = atoi(argv[2]);
    }
    else
    {
        printf("Please enter the name of the file to read in and whether you want verbose debug printouts.\n");
        return 0;
    }

    printf("You entered '%s' and '%d' for verbose.\n", fileName, verbose);
    // Instantiate the two caches

   
        
    /*Scenario 1: Load all of the cache lines of a particular set
     
     1.  Make mem space for data (8 way assoc and 1MB
     
     2.  Set up instrcution cache (read vs write  - 4way assos - 1MB)
     
     3. Read in entire set.
        -> call function to get set
     
     While(!eof)
            4.  read indivdual lines and Parce one at a timne
            
                 -> Call to line function (Class encompasses below info)
                        -> call mode function (to read first byte of line)
                           -> convert remainder of array to hex
                               -> Call Tag function
                                 -> call index function 
                                   -> call to the offset
                         ->  Check Mesi bits 
                                -> Determine hit or miss 
                                    -> check tag matches
                                      Return (hit or miss) 
                                    
                     
        
           
      


*/ 
    return 1;
}
