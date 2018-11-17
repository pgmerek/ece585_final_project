/* ECE 485/585 Fall Term 2018 Final Project
 * Split L1 Cache simulation
 * Members: Patrick Gmerek, Stephen Poanessa, Emma Smith, Amanda Voegtlin
 * 4 November 2018
 */

#include "header.h"

//Arg = mode (0 =summary of staticscs, 1= verbose)
int main(int argc, char * argv[])
{
    char fileName[BUFFER_SIZE];
    bool verbose = false;
    traces * references = NULL;

    if (argc == 3)
    {
        strcpy(fileName, argv[1]);
        verbose = atoi(argv[2]);
    }
    else
    {
        printf("Please enter the name of the file to read in and whether you want verbose debug printouts.\n");
        return 0;
    }
    // Read data from file
    if (!read_file(references, fileName))
    {
        printf("An error occured when reading the file. Exiting....\n");
        return 0;
    }
    // Instantiate the two caches
    cache data(DATA_NUM_LINES);
    cache instruction(INSTR_NUM_LINES);

   
        
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


int read_file(traces * references, char * fileName)
{
    char buffer[BUFFER_SIZE];
    int count = 0;
    int i = 0;  // Used to iterate through the references
    bool error = false;
    ifstream in;   // Open a new read-only file stream
    in.open(fileName);

    if (!in)    // If the file doesn't exist, return an error
        return 0;

    while (!in.eof())   // Get number of lines
    {
        in.get(buffer, BUFFER_SIZE, '\n');
        in.get();   // Pauses between lines
        ++count;
        if (in.eof())   // Prevents an off by one error
            --count;
    }

    in.clear();     // Clear eof flag
    in.seekg (0, ios::beg); // Return to beginning of file
    references = new traces[count]; // Create a dynamic array of traces

    for (int k = 0; k < count; ++k)
    {
        in.get(buffer, BUFFER_SIZE, '\n');  // Read each line
        in.get();   // Chills bruh
        references[i++].populate(buffer);
    }
    in.close();

    if (error)
    {
        if (references) // Prevent memory leak
            delete [] references;
        return 0;
    }
    
    return 1;
}

