

#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

//comment out if building on linux.
#define WINBUILD

// if memory should loop back on it's self by default when created. (if used as a standalone system this will usually be yes(=1))
#define MEMSPACE_LOOP_DEFAULT 1

//if memory should be accessed byte by byte or in chunks.
#define USE_BYTEWISE_RW

//if you want a lot of debug messages in your console.
//#define DBG_MESSAGES
//#define VERBOSE_DBG

//if you want to build the windows specific memory accessor construct page.
#define BUILD_WIN_MEMACCESSOR

//if you want the program to beep on errors.
#define DEBUG_BEEP

#endif // CONFIG_H_INCLUDED
