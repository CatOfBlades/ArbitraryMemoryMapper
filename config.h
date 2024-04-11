

#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

#ifndef LINUX_BUILD
#define WINBUILD
#endif

//#define LUA_USE_POSIX

// if memory should loop back on it's self by default when created. (if used as a standalone system this will usually be yes(=1))
#define MEMSPACE_LOOP_DEFAULT 0

//if memory should be accessed byte by byte or in chunks.
#define USE_BYTEWISE_RW

//if you want a lot of debug messages in your console.
#define DBG_MESSAGES
#define VERBOSE_DBG

/** Begin WinMem Specific Settings **/
//if you want to build the windows specific memory accessor construct page.
#ifndef LINUX_BUILD
#define BUILD_WIN_MEMACCESSOR
#endif

//#define PROGRAM_MIN_WORKING_SET_SIZE 204800 //minimum memory in bytes.
//#define PROGRAM_MAX_WORKING_SET_SIZE 1413120 //maximum memory in bytes.

#define PROGRAM_MIN_WORKING_SET_SIZE 0 //minimum memory in bytes.
#define PROGRAM_MAX_WORKING_SET_SIZE 2826240 //maximum memory in bytes.
/** End WinMem Specific Settings **/

//if you want the program to beep on errors.
//#define DEBUG_BEEP

//if you want to build in the split byte reader that returns a weighted floating point value between two addresses.
//#define SUPPORT_UNUSUAL_MEMORY_ACCESSES

#ifndef LINUX_BUILD
#define BUILT_IN_VISUALIZER
#endif

#ifdef __STDC_LIB_EXT1__ //checks to see if lib stdc extended functions are avalible.
#define __STDC_WANT_LIB_EXT1__
#endif

#endif // CONFIG_H_INCLUDED
