

#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

//comment out if building on linux.
#define WINBUILD

// if memory should loop back on it's self by default when created. (if used as a standalone system this will usually be yes(=1))
#define MEMSPACE_LOOP_DEFAULT 1

//if memory should be accessed byte by byte or in chunks.
//#define USE_BYTEWISE_RW

//if you want a lot of debug messages in your console.
#define DBG_MESSAGES
//#define VERBOSE_DBG

/** Begin WinMem Specific Settings **/
//if you want to build the windows specific memory accessor construct page.
#define BUILD_WIN_MEMACCESSOR

//#define PROGRAM_MIN_WORKING_SET_SIZE 204800 //minimum memory in bytes.
//#define PROGRAM_MAX_WORKING_SET_SIZE 1413120 //maximum memory in bytes.

#define PROGRAM_MIN_WORKING_SET_SIZE 0 //minimum memory in bytes.
#define PROGRAM_MAX_WORKING_SET_SIZE 2826240 //maximum memory in bytes.
/** End WinMem Specific Settings **/

//if you want the program to beep on errors.
#define DEBUG_BEEP

//if you want to build in the split byte reader that returns a weighted floating point value between two addresses.
#define SUPPORT_UNUSUAL_MEMORY_ACCESSES

#define BUILT_IN_VISUALIZER

#endif // CONFIG_H_INCLUDED
