
#ifndef __CU_TARRAY_H_
#define __CU_TARRAY_H_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <limits.h>

#include "CUtils.h"

typedef void (* Array_OnVisit_Callback)(void *, int32_t);

// Forward declaration
struct Array;
typedef struct Array Array;

typedef struct Array_ClassFunctions
{
    /**
     * @return  The element at the provided index.
     */
    void * (* const GetAt)(Array *Self, int32_t Index);

    /**
     * Appends an element at the array. Can re-allocate the internal array.
     */
    void (* const Add)(Array *Self, void *Element);

    /**
     * Appends the other elements to this array. The 'Count' must not take into account the 'Sizeof' element
     * but it must be the real nmber of elements to copy. The conversion from count to the count of bytes
     * will be done by this.
     */
    void (* const Append)(Array *Self, void *Other, uint32_t Count);

    /**
     * Same as 'Append' but it does a raw copy to the buffer, with the provided 'ByteSize'.
     */
    void (* const AppendRaw)(Array *Self, void *Other, uint32_t ByteSize);

    /**
     * Removes an element at the specified index. It instead of moving all the array around
     * it will swap the element at 'Index' with the last element.
     */
    void (* const Remove)(Array *Self, int32_t Index);

    /**
     * @return  The count of elements in the array.
     */
    uint32_t (* const Count)(Array *Self);

    /**
     * @return  The count of unused array elements.
     */
    uint32_t (* const GetSlackElements)(Array *Self);

    /**
     * @return  The count of unused array bytes
     */
    uint32_t (* const GetSlackBytes);

    /**
     * @return  True if the index is valid and usable, and false otherwise.
     */
    bool (* const IsValidIndex)(Array *Self, int32_t Index);

    /**
     * Ensures the inner size of the array buffer to hold.
     */
    void (* const EnsureCapacity)(Array *Self, uint32_t ExtraElementCount);

    /**
     * Compacts all slack.
     */
    void (* const Compact)(Array *Self);
} Array_ClassFunctions;

/**
 * Private members, do not modify directly! Access only!
 */
typedef struct Array_Private
{
    // Size of one element that will fit in the array
    size_t SizeofElement;

    // Inner buffer
    uint8_t *Buffer;
    size_t BufferSize;

    // Index used within the buffer. Count is this index divided by 'ElementSize'
    size_t BufferIndex;

} Array_Private;

/**
 * Simulated 'template' c array ADT. Quite memory heavy, don't over-use!
 */
typedef struct Array
{
    // Don't modfy private! Access only!
    Array_Private Private;

    // Do not tamper with this!
    Array_ClassFunctions *Ft; // Function table
} Array;

// Statically (stack) fct defined reader
void NewArrayStatic(Array *Array, uint32_t SizeofElement);
void DeleteArrayStatic(Array *Array);

// Malloc created reader
Array *NewArray(uint32_t SizeofElement);
void DeleteArray(Array *Array);

#define DELETE_ARRAY_SAFE(a) do { DeleteArray(a); a = NULL; } while(0)

#endif /* __CU_TARRAY_H_ */
