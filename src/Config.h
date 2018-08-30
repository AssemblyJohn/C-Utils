

#ifndef __CU_CONFIG_H_
#define __CU_CONFIG_H_

// Define debug mode
#define __CU_UTILS_DEBUG


// INTERNAL

#include <stdlib.h>
#include <string.h>

#include "Platform.h"

/* Printing utilities */

#ifdef __CU_UTILS_DEBUG
    #define ERROR(...) printf(__VA_ARGS__)
#else
    #define ERROR(...) do {} while(0)
#endif

#ifdef __CU_UTILS_DEBUG
    #define PRINT(...) printf(__VA_ARGS__)
#else
    #define PRINT(...) do {} while(0)
#endif

/* Type safe memory utilities. Inlined, don't worry about call overhead */


// For use on systems that do not have the malloc/realloc like bootloaders, and where we need to quickly define other fcts
INLINE FORCEINLINE void *CU_Malloc(size_t size)
{
    return malloc(size);
}


INLINE FORCEINLINE void *CU_Realloc(void *ptr, size_t size)
{
    return realloc(ptr, size);
}

INLINE FORCEINLINE void CU_Free (void *ptr)
{
	free(ptr);
}

INLINE FORCEINLINE void *CU_Memcpy(void *RESTRICT dest, const void *RESTRICT src, size_t n)
{
    return memcpy(dest, src, n);
}

#endif /* __CU_CONFIG_H_ */
