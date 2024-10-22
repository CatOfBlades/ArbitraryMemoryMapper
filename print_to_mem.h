/***********************************************************\
 * A function to print text directly into a memory space.
 * Instead of writing to the terminal, it writes to the
 * specified region in memory.
\***********************************************************/

#ifndef PRINT_TO_MEM_H_INCLUDED
#define PRINT_TO_MEM_H_INCLUDED

#include "virtualMemorySpace.h"
#include <string>

/*
 * Writes a string to a specified memory region within the virtualMemorySpace.
 *
 * Parameters:
 *    vms - The virtualMemorySpace instance to write to.
 *    start_print_region - The start address of the memory region to write to.
 *    end_print_region - The end address of the memory region to write to.
 *    txt - The string to write into memory.
 *    len - The length of the string to write.
 *
 * Returns:
 *    0 on success, non-zero otherwise.
 */
int print_to_mem(virtualMemorySpace vms, unsigned long start_print_region, unsigned long end_print_region, const char* txt, size_t len);

#endif // PRINT_TO_MEM_H_INCLUDED
