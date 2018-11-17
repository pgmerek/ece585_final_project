/* ECE 485/585 Fall Term 2018 Final Project
 * Split L1 Cache simulation
 * Members: Patrick Gmerek, Stephen Poanessa, Emma Smith, Amanda Voegtlin
 * 4 November 2018
 */

#include "header.h"

int main(int argc, char * argv[])
{
    char file_name[BUFFER_SIZE];
    bool verbose = false;
    int num_lines = 0;
    int operation = -1;         // Used when processing 
    int address = -1;           // from file to cache
    traces * references = NULL;

    if (argc == 3)
    {
        strcpy(file_name, argv[1]);
        verbose = atoi(argv[2]);
    }
    else
    {
        printf("USAGE: ./cache_simulator <name of trace file> <verbosity>\n");
        printf("where...\n\tname of trace file is a string\n\tverbosity is either 0 for false, or 1 for true\n");
        return 0;
    }
    // Read data from file
    num_lines = read_file(&references, file_name, verbose);
    if (num_lines < 0 || !references)
    {
        printf("An error occured when reading the file. Exiting....\n");
        return 0;
    }
    // Instantiate the two caches
    cache data(DATA_NUM_LINES);
    cache instruction(INSTR_NUM_LINES);

    for (int k = 0; k < num_lines; ++k)
    {
        operation = references[k].get_operation();
        address = references[k].get_address();
        
        switch (operation)
        {
            case 0: // Read data request, sent to L1 from memory
            
                break;
            case 1: // Write to L1 data cache, sent to L1 from memory
                break;
            case 2: // Read from instruction cache
                break;
            case 3: // Invalidate from L2
                break;
            case 4: // Data request from L2
                break;
            case 5: // Invalid
                printf("Operation invalid. %d is not a valid operation. Exiting....\n", operation);
                return -1;
            case 6: // Invalid
                printf("Operation invalid. %d is not a valid operation. Exiting....\n", operation);
                return -1;
            case 7: // Invalid
                printf("Operation invalid. %d is not a valid operation. Exiting....\n", operation);
                return -1;
            case 8: // Clear cache and reset all statistics
                break;
            case 9: // Print contents and state of the cache
                break;
            default:
                printf("Operation invalid. %d is not a valid operation. Exiting....\n", operation);
                return -1;
        }
    }
    
    // Print hit rate, miss rate, ratio of hits to misses, read operations, write operations, and all operations

        
    return 1;
}


int read_file(traces ** references, char * file_name, bool verbose)
{
    char buffer[BUFFER_SIZE];
    int count = 0;
    int k = 0;
    bool error = false;
    traces * temp = NULL;
    ifstream in;   // Open a new read-only file stream
    in.open(file_name);

    if (!in)    // If the file doesn't exist, return an error
        return -1;

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
    
    temp = new traces [count]; // Create a dynamic array of traces
    if (verbose)
            printf("Reading traces from %s.\n", file_name);

    while (!error && k < count)
    {
        in.get(buffer, BUFFER_SIZE, '\n');  // Read each line
        in.get();   // Chills bruh
        if (verbose)
            printf("Trace %d: ", k);
        error = temp[k++].populate(buffer, verbose);
    }
    in.close();

    if (error)
    {
        if (temp) // Prevent memory leak
            delete [] temp;
        temp = NULL;
        return -1;
    }

    *references = temp;     // Return the traces to main
    
    return count;
}

