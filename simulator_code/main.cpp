/* ECE 485/585 Fall Term 2018 Final Project
 * Split L1 Cache simulation
 * Members: Patrick Gmerek, Stephen Poanessa, Emma Smith, Amanda Voegtlin
 * 4 November 2018
 */

#include "header.h"

int main(int argc, char * argv[])
{
    char file_name[BUFFER_SIZE];
    char msg_buffer[BUFFER_SIZE];
    int verbose = false;
    int num_traces = 0;
    // Used when processing file to cache
    int operation = -1;
    int raw_address = -1;
    traces * references = NULL;
    cache_messages messages;
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

        if (verbose == 2)
            printf("=======On case number %d, address %x. ", k, raw_address);
        
        switch (operation)
        {
            case 0: // Read data request, sent to L1 from memory
                if (verbose == 2)
                    printf("Read data request.=======\n");
                data.increment_reads();
                // If entry is in the data cache
                if (data.contains(temp_entry, operation, verbose))
                {
                    data.increment_hits();
                    if (verbose == 2)
                        printf("Hit\n");
                }
                // If the entry is not in the data cache
                else
                {
                    if (verbose == 2)
                        printf("Miss\n");
                    
                    data.increment_misses();
                    // Add it to the cache
                    if (!data.miss_handler(temp_entry, operation, messages, verbose))
                    {
                        if (verbose == 2)
                            printf("An error occured when reading from the data cache.\n");
                    }
                    // Write through the first line if the cache is empty
                    if (data.get_misses() == 1 && data.get_hits() == 0)
                    {
                        // Add return data to L2 message to the list of messages 
                        sprintf(msg_buffer, "%s%x", WRITE_TO_L2, temp_entry.get_raw_address());
                        messages.add_message(msg_buffer);
                    }
                }
                break;
            case 1: // Write to L1 data cache, sent to L1 from memory
                if (verbose == 2)
                    printf("Write data request.=======\n");
                data.increment_writes();
                // If entry is in the data cache
                if (data.contains(temp_entry, operation, verbose))
                {
                    data.increment_hits();
                    // Write back to L2 on hit
                    sprintf(msg_buffer, "%s%x", WRITE_TO_L2, temp_entry.get_raw_address());
                    messages.add_message(msg_buffer);
                    if (verbose == 2)
                        printf("Hit\n");
                }
                // If the entry is not in the data cache
                else
                {
                    if (verbose == 2)
                        printf("Miss\n");

                    data.increment_misses();
                    // Add it to the cache
                    if (!data.miss_handler(temp_entry, operation, messages, verbose))
                    {
                        if (verbose ==2)
                        printf("An error occured when writing to the data cache.\n");
                    }
                    // Write through the first line if the cache is empty
                    if (data.get_misses() == 1 && data.get_hits() == 0)
                    {
                        // Add return data to L2 message to the list of messages 
                        sprintf(msg_buffer, "%s%x", WRITE_TO_L2, temp_entry.get_raw_address());
                        messages.add_message(msg_buffer);
                    }
                }
                break;
            case 2: // Read from instruction cache
                if (verbose == 2)
                    printf("Read instruction request.=======\n");
                instruction.increment_reads();
                // If entry is in the instruction cache
                if (instruction.contains(temp_entry, operation, verbose))
                {
                    instruction.increment_hits();
                    if (verbose == 2)
                        printf("Hit\n");
                }
                // If the entry is not in the instruction cache
                else
                {
                    if (verbose == 2)
                        printf("Miss\n");
                    instruction.increment_misses();
                    // Add it to the cache
                    if (!instruction.miss_handler(temp_entry, operation, messages, verbose))
                    {
                        if (verbose == 2)
                            printf("An error occured when reading from the instruction cache.\n");
                    }
                     // Write through first line if the cache is empty
                    if (instruction.get_misses() == 1 && instruction.get_hits() == 0)
                    {
                        // Add return data to L2 message to the list of messages 
                        sprintf(msg_buffer, "%s%x", WRITE_TO_L2, temp_entry.get_raw_address());
                        messages.add_message(msg_buffer);
                    }
                }
                break;
            case 3: // Invalidate from L2
                if (verbose == 2)
                    printf("Invalidate from L2 request.=======\n");

                // Invalidate any matching entry in the data cache
                if (!data.invalidate_entry(temp_entry, messages, verbose))
                    printf("Could not invalidate %x in the data cache because it's not there.\n", temp_entry.get_raw_address());
                // Invalidate any matching entry in the instruction cache
                if (!instruction.invalidate_entry(temp_entry, messages, verbose))
                    printf("Could not invalidate %x in the instruction cache because it's not there.\n", temp_entry.get_raw_address());
                break;
            case 4: // Data request from L2
                if (verbose == 2)
                    printf("Data request from L2.=======\n");

                // Search for entry and update mesi
                if (data.read_request(temp_entry, messages, verbose) == -1)
                    printf("An error occured when executing the data request from L2.\n");
                break;
            case 8: // Clear cache and reset all statistics
                if (verbose == 2)
                    printf("Clear cache request.=======\n");

                // Clear statistics for both caches and delete all L2 messages
                data.clear(verbose);
                instruction.clear(verbose);   
                messages.delete_messages();
                break;
            case 9: // Print contents and state of the cache
                if (verbose == 2)
                    printf("Print contents request.=======\n");
                printf("\n==================== Summary Statistics =================\n");
                printf("--------- Data Cache ---------\n");
                data.print_statistics();
                printf("\n--------- Instruction Cache ---------\n");
                instruction.print_statistics();
                printf("\n======================== Data Cache Contents =======================\n");
                printf("\nKey: LRU = 0 and MRU = 7\n");
                data.print_contents();
                printf("\n==================== Instruction Cache Contents ====================\n");
                printf("\nKey: LRU = 0 and MRU = 3\n");
                instruction.print_contents();
                if (verbose)
                {
                    printf("\n========================== Messages to L2 ==========================\n");
                    messages.display_messages();
                }
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

