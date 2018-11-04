/* ECE 485/585 Fall Term 2018 Final Project
 * Split L1 Cache simulation
 * Members: Patrick Gmerek, Stephen Poanessa, Emma Smith, Amanda Voegtlin
 * 4 November 2018
 */

#include <header.h>

//Arg = mode (0 =summary of staticscs, 1= verbose)
int main(int arg c, char * argv[])
{
    

    /*Scenario 1: Load all of the cache lines of a particular set
     
     1.  Make mem space for data 
     
     2.  Set up instrcution cache (read vs write)
     
     3. Read in entire set.
        -> call function to get set
     
     While(# lines >=0)
            4.  read indivdual lines and Parce one at a timne
            
                 -> Call to line function
                        -> call mode function (to read first byte of line)
                           -> convert remainder of array to hex
                     
        
                    ->  Call to mode function (is read or write)
                       ->  Call 
             
        
     3. 
     
     
    
    1. Get char for operation type
        
    2. get hex value one space away on sam line
3. convert hex value to a binary string
4. Read char 0-6 to get byte offset
5. read char 7-20 for index
6. read chars 21-32 for tag
7. Load "way" in index
8. Update LRU
9. Update MESI


*/ 
