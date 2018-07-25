#include "Config.h"

// Change here depending on the platform

void *CU_Malloc (size_t size)
{
    return malloc(size);
};

void *CU_Realloc(void *ptr, size_t size)
{
    return realloc(ptr, size);
}

void CU_Free (void *ptr)
{
    free(ptr);
}

void *CU_Memcpy(void *__restrict dest, const void *__restrict src, size_t n)
{
    return memcpy(dest, src, n);
}
