
#include "print_to_mem.h"

int print_to_mem(virtualMemorySpace vms, unsigned long start_print_region, unsigned long end_print_region, const char* txt, size_t len) {
    // Ensure the string length does not exceed the memory region
    if (start_print_region + len > end_print_region) {
        return -1;  // Return error if the string overflows the allowed memory region
    }

    // Write the string to memory using the virtualMemorySpace's writeMem function
    unsigned long written_bytes = vms.writeMem(start_print_region, (unsigned char*)txt, len);

    // Check if all bytes were successfully written
    if (written_bytes != len) {
        return -2;  // Return error if the entire string could not be written
    }

    return 0;  // Success
}
