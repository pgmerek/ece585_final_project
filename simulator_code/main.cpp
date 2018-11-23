/* ECE 485/585 Fall Term 2018 Final Project
 * Split L1 Cache simulation
 * Members: Patrick Gmerek, Stephen Poanessa, Emma Smith, Amanda Voegtlin
 * 4 November 2018
 */

#include "header.h"

int main(int argc, char * argv[])
{
    char file_name[BUFFER_SIZE];
    int verbose = false;
    int num_traces = 0;
    // Used when processing file to cache
    int operation = -1;
    int raw_address = -1;
    int address = -1;
    int tag = -1;
    int index = -1;
    int offset = -1;
    traces * references = NULL;
    entry temp_entry;

    if (argc == 3)
    {
        strcpy(file_name, argv[1]);
        verbose = atoi(argv[2]);
    }
    else
    {
        printf("USAGE: ./cache_simulator <name of trace file> <verbosity>\n");
        printf("where...\n\tname of trace file is a string\n\tverbosity is from 0 to 2 from few messages to many messages\n");
        return 0;
    }
    // Read data from file
    num_traces = read_file(&references, file_name, verbose);
    if (num_traces < 0 || !references)
    {
        printf("An error occured when reading the file. Exiting....\n");
        return 0;
    }
    // Instantiate the two caches
    cache data(DATA_NUM_LINES);
    cache instruction(INSTR_NUM_LINES);

    for (int k = 0; k < num_traces; ++k)
    {
        operation = references[k].get_operation();
        raw_address = references[k].get_address();
        temp_entry.populate_entry(raw_address, verbose);
        tag = temp_entry.get_tag();
        index = temp_entry.get_index();
        offset = temp_entry.get_offset();

        if (verbose == 2)
            printf("=======On case number %d, address %x. ", k, raw_address);
        
        switch (operation)
        {
            case 0: // Read data request, sent to L1 from memory
                if (verbose == 2)
                    printf("Read data request.=======\n");
                data.increment_reads();
                if (data.contains(temp_entry, verbose))
                {
                    data.increment_hits();
                    printf("Hit\n");
                }
                else
                {
                    data.increment_misses();
                    if (!data.miss_handler(temp_entry, operation, verbose))
                        printf("An error occured in the read miss handler.\n");
                    else
                        printf("Miss\n");
                }
                break;
            case 1: // Write to L1 data cache, sent to L1 from memory
                if (verbose == 2)
                    printf("Write data request.=======\n");
                data.increment_writes();
                if (data.contains(temp_entry, verbose))
                {
                    data.increment_hits();
                    printf("Hit\n");
                }
                else
                {
                    data.increment_misses();
                    if (!data.miss_handler(temp_entry, operation, verbose))
                        printf("An error occured when reading from the data cache.\n");
                    else
                        printf("Miss\n");
                }
                // Might need to invalid copy in L2 if write miss and entry is shared
                break;
            case 2: // Read from instruction cache
                if (verbose == 2)
                    printf("Read instruction request.=======\n");
                instruction.increment_reads();
                if (instruction.contains(temp_entry, verbose))
                {
                    instruction.increment_hits();
                    printf("Hit\n");
                }
                else
                {
                    instruction.increment_misses();
                    if (!instruction.miss_handler(temp_entry, operation, verbose))
                        printf("An error occured when reading from the instruction cache.\n");
                    else
                        printf("Miss\n");
                }
                break;
            case 3: // Invalidate from L2
                if (verbose == 2)
                    printf("Invalidate from L2 request.=======\n");
                // Need to search for an entry in the cache. if found, invalidate it.
                // Refer to snoop diagram for logic on updating hits/misses and writing back to L2 (just a print statement)
                switch (data.invalidate_snoop(temp_entry, verbose)) // Invalidate it and capture return value
                {
                    case -1:    // The entry was not found when executing cache.invalidate_snoop
                        printf("Entry not invalidated since it wasn't found in the cache.\n");
                        break;
                    case 0: // Invalidated from shared, invalid, or exclusive state. Don't increment hits
                        break;
                    case 1: // Only increment hits if invalidating from modified state
                        data.increment_hits();
                        break;
                }
                break;
            case 4: // Data request from L2
                if (verbose == 2)
                    printf("Data request from L2.=======\n");
                break;
            case 8: // Clear cache and reset all statistics
                if (verbose == 2)
                    printf("Clear cache request.=======\n");
                data.clear(verbose);
                instruction.clear(verbose);   
                break;
            case 9: // Print contents and state of the cache
                if (verbose == 2)
                    printf("Print contents request.=======\n");
                data.print_contents();
                break;
            default:
                printf("Operation invalid. %d is not a valid operation. Exiting....\n", operation);
                return -1;
        }
    }
    
    if (references)
        delete [] references;
    return 1;
}


int read_file(traces ** references, char * file_name, int verbose)
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
    if (verbose == 2)
            printf("Reading traces from %s.\n", file_name);

    while (!error && k < count)
    {
        in.get(buffer, BUFFER_SIZE, '\n');  // Read each line
        in.get();   // Chills bruh
        if (verbose == 2)
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

