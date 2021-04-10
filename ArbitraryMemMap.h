

#ifndef ARBITRARYMEMMAP_H_INCLUDED
#define ARBITRARYMEMMAP_H_INCLUDED

/**
Okay so the idea is to make a common interface for refrencing arbitrary memory mappings.
This way I can pretend that memory spaces in other applications or even other devices are within the same address range.

This allows me to sorta undo ASLR(address space layout randomization)
by manually listing every accessable region of memory.

And as new regions of memory become avalible, treating them as if they are in the same memory space.
**/

/**
	example of creating a new interface would be to create a subclass of 'addressSpace' (defined in addressSpace.h)
	and defining the virtual functions. (as seen in virtualPage.h) //virtual page defines unencrypted local application memory mappings within the current running process.
	Each virtual function tells the 'virtualMemorySpace' object how reads and writes should be handled.
**/

#include "Defines.h"
#include "AMM/addressSpace.h"
#include "AMM/virtualPage.h"
#include "AMM/virtualMemorySpace.h"
#ifdef USINGPAGELOGGING
#include "AMM/LoggedPage.h"
#endif
#ifdef USINGINTERPROCESS
    #include "AMM/InterprocessMemorySpace.h"
#endif
#ifdef USING_SELF_REFRENCE
    #include "AMM/selfRefrence.h"
#endif
#ifdef USING_MULTI_PAGE
    #include "AMM/multiPage.h"
#endif
#ifdef USING_LUA_PAGE
    #include "AMM/LuaPage.h"
#endif
#ifdef USING_FILE_PAGE
    #include "AMM/filePage.h"
#endif


#endif // ARBITRARYMEMMAP_H_INCLUDED
