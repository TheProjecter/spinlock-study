#ifndef INCLUDED_PLATFORM
#define INCLUDED_PLATFORM


/** One of each of the following groups of definitions must be uncommented **/

#define WINDOWS_SYSTEM
//#define UNIX_SYSTEM
//#define SOLARIS_SYSTEM

#define OPA_HAVE_GCC_INTRINSIC_ATOMICS
//#define OPA_HAVE_GCC_X86_32_64
//#define OPA_HAVE_SUN_ATOMIC_OPS

#define OPA_SIZEOF_INT 4
//#define OPA_SIZEOF_INT 8

#define CACHE_LINE_SIZE_BYTES 128
#define CACHE_LINE_INT_SKIP (CACHE_LINE_SIZE_BYTES/sizeof(int))

#define MAX_THREADS 1024

/** end definition selection **/


#include "opa_primitives.h"

#if defined(WINDOWS_SYSTEM)

#include <windows.h>

void (*sleep_fn)(int ms) = (void (*)(int)) Sleep;

LARGE_INTEGER __TICKS_PER_SECOND;
BOOL dummy = QueryPerformanceFrequency(&__TICKS_PER_SECOND);

double time_sec() {
    LARGE_INTEGER t;
    QueryPerformanceCounter(&t);
    return (t.QuadPart / (double) __TICKS_PER_SECOND.QuadPart);
}

#elif defined(UNIX_SYSTEM)

#include <unistd.h>
#include <sys/time.h>

void (*sleep_fn)(int ms) = (void (*)(int)) sleep;

double time_sec() {
    timeval t;
    gettimeofday(&t, NULL);
    return (t.tv_sec + t.tv_usec / 1000000.);
}

#elif defined(SOLARIS_SYSTEM)

#include <atomics.h>

#else
#error no recognized system type defined
#endif

#endif
