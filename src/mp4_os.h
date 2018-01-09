#ifndef MP4_OS_H
#define MP4_OS_H

#ifdef _ANDROID_
#include <futils/futils.h>
#else

#include <stdio.h>
#include "mp4_list.h"
#define ULOG_DEBUG 1
#define ULOG_PRI(level,...) printf(__VA_ARGS__)

#if  defined _WIN32
typedef __int64 off_t;
#define fseeko _fseeki64
#define ftello _ftelli64
#else
#endif

#endif

#endif