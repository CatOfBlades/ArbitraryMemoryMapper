
#include "ArbitraryMemMap.h"


/**
Okay so the idea is to make a common interface for refrencing arbitrary memory mappings.
This way I can pretend that memory spaces in other applications or even other devices are within the same address range.

This allows me to sorta undo ASLR(address space layout randomization)
by manually listing every accessable region of memory.

And as new regions of memory become avalible, treating them as if they are in the same memory space.
**/

/**
	example of creating a new interface would be to create a subclass of 'addressSpace'
	and defining the virtual functions.
	Each virtual function tells the 'virtualMemorySpace' object how reads and writes should be handled.
**/




