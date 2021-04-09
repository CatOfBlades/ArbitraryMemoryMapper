#ifndef DEFINES_H_INCLUDED
#define DEFINES_H_INCLUDED

#ifdef _WIN32
#define WINBUILD
#endif
#ifdef _WIN64
#define WINBUILD
#endif

// if memory should loop back on it's self by default when created. (if used as a standalone system this will usually be yes(=1))
#define MEMSPACE_LOOP_DEFAULT 1
#define USE_BYTEWISE_RW


#endif // DEFINES_H_INCLUDED
