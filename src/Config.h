

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
INLINE extern void *CU_Malloc(size_t size) FORCEINLINE;
INLINE extern void *CU_Realloc(void *ptr, size_t size) FORCEINLINE;
INLINE extern void CU_Free (void *) FORCEINLINE;

INLINE extern void *CU_Memcpy(void *__restrict dest, const void *__restrict src, size_t n) FORCEINLINE;

#endif /* __CU_CONFIG_H_ */
