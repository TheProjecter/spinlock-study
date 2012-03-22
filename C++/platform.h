#ifndef INCLUDED_PLATFORM
#define INCLUDED_PLATFORM


/** One of each of the following groups of definitions must be uncommented **/

//#define WINDOWS_SYSTEM
#define UNIX_SYSTEM
//#define SOLARIS_SYSTEM

#define OPA_HAVE_GCC_INTRINSIC_ATOMICS
//#define OPA_HAVE_GCC_X86_32_64
//#define OPA_HAVE_SUN_ATOMIC_OPS

#define OPA_SIZEOF_INT 4
//#define OPA_SIZEOF_INT 8

#define CACHE_LINE_SIZE_BYTES 128

/** end definition selection **/


#include "opa_primitives.h"

#if defined(WINDOWS_SYSTEM)
#include <windows.h>
#elif defined(UNIX_SYSTEM)
#include <unistd.h>
#elif defined(SOLARIS_SYSTEM)
#include <atomics.h>
#else
#error no recognized system type defined
#endif

#endif
