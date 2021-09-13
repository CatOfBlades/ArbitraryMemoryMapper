
#ifndef DEFINES_H_INCLUDED
#define DEFINES_H_INCLUDED

#include "config.h"

/*
#ifdef _WIN32
#define WINBUILD
#endif
#ifdef _WIN64
#define WINBUILD
#endif
*/

#ifndef WINBUILD
#define Sleep(x) usleep(x)
#endif

#ifdef DBG_MESSAGES
#ifdef VERBOSE_DBG
#define EXTRA_DEBUG_MESSAGES
#endif
#endif



#endif // DEFINES_H_INCLUDED
