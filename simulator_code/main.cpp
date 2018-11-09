/* ECE 485/585 Fall Term 2018 Final Project
 * Split L1 Cache simulation
 * Members: Patrick Gmerek, Stephen Poanessa, Emma Smith, Amanda Voegtlin
 * 4 November 2018
 */

#include "header.h"

//Arg = mode (0 =summary of staticscs, 1= verbose)
int main(int argc, char * argv[])
{
    
  

	char const* const fileName = "test.txt"; 
	FILE* file = fopen(fileName, "r"); 
	char line[500];

	while (fgets(line, sizeof(line), file)) {
	printf("%s", line); 
	}
fclose(file);
 
        
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
}
