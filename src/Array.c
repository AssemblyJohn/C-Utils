#include "Array.h"

const float ARRAY_PRECENT_GROW = .3f;

// INTERNAL FUNCTIONS

static void Array_Ctor(Array *Self, uint32_t SizeofElement)
{
    Self->Private.SizeofElement = SizeofElement;

    // Starts with an empty buffer
    Self->Private.Buffer = NULL;
    Self->Private.BufferIndex = 0;
    Self->Private.BufferSize = 0;
}

static void Array_Ctor_1(Array *Self, uint32_t SizeofElement, uint32_t Reserved)
{
    Self->Private.SizeofElement = SizeofElement;

    // TODO:
}

static void Array_Dtor(Array *Self)
{
    // Clean up
    if(Self->Private.Buffer != NULL)
    {
        free(Self->Private.Buffer);
        Self->Private.Buffer = NULL;

        Self->Private.BufferSize = 0;
        Self->Private.BufferIndex = 0;
    }
}

static uint32_t Array_GetSlackElements(Array *Self)
{
    return ((Self->Private.BufferSize - Self->Private.BufferIndex) / Self->Private.SizeofElement);
}

static uint32_t Array_GetSlackBytes(Array *Self)
{
    return (Self->Private.BufferSize - Self->Private.BufferIndex);
}

static uint32_t Array_LastIndex(Array *Self)
{
    return ((Self->Private.BufferIndex / Self->Private.SizeofElement) - 1);
}

static uint32_t Array_Count(Array *Self)
{
    return (Self->Private.BufferIndex / Self->Private.SizeofElement);
}

static bool Array_IsValidIndex(Array *Self, int32_t Index)
{
    uint32_t RealIndex = Index * Self->Private.SizeofElement;
    return (RealIndex >= 0) && (RealIndex < Self->Private.BufferIndex);
}

static void Array_EnsureCapacity(Array *Self, uint32_t ExtraElementCount)
{
    if(Array_GetSlackElements(Self) < ExtraElementCount)
    {
        // Make larger the internal buffer
        uint32_t Count = Array_Count(Self);

        uint32_t NewSize;

        if(abs(Count - ExtraElementCount) == 1)
            NewSize = (1 + Count + ((uint32_t)((float)Count * ARRAY_PRECENT_GROW))) * Self->Private.SizeofElement;
        else
            NewSize = Self->Private.SizeofElement * ExtraElementCount;

        // Realloc the internal buffer
        Self->Private.Buffer = realloc(Self->Private.Buffer, NewSize);
        Self->Private.BufferSize = NewSize;
    }
}

static void * Array_GetAt(Array *Self, int32_t Index)
{
    if(Array_IsValidIndex(Self,Index))
    {
        uint32_t RealIndex = Index * Self->Private.SizeofElement;
        return (void *)(Self->Private.Buffer + RealIndex);
    }

    return NULL;
}

static void Array_Add(Array *Self, void *Element)
{
    // Ensure capacity
    Array_EnsureCapacity(Self, 1);

    // Copy the data in the array & increase the buffer index
    memcpy(Self->Private.Buffer + Self->Private.BufferIndex, Element, Self->Private.SizeofElement);
    Self->Private.BufferIndex += Self->Private.SizeofElement;
}

static void Array_Remove(Array *Self, int32_t Index)
{
    if(Array_IsValidIndex(Self, Index))
    {
        // If it is not the last index
        if(Index != (Array_Count(Self) - 1))
        {
            // Copy the last element over this one that we'll delete
            uint32_t RealIndex = Index * Self->Private.SizeofElement;
            uint32_t RealIndexLast = Array_LastIndex(Self) * Self->Private.SizeofElement;

            memcpy(Self->Private.Buffer + RealIndex, Self->Private.Buffer, Self->Private.SizeofElement);

            // Decrement the buffer index
            Self->Private.BufferIndex -= Self->Private.SizeofElement;
        }
        else
        {
            // Simply decrement the buffer index if it is the last element
            Self->Private.BufferIndex -= Self->Private.SizeofElement;
        }
    }
}

static void Array_Append(Array *Self, void *Other, uint32_t Count)
{
    // Ensure capacity
    Array_EnsureCapacity(Self, Count);

    uint32_t CopySize = Count * Self->Private.SizeofElement;

    // Copy the data in the array & increase the buffer index
    memcpy(Self->Private.Buffer + Self->Private.BufferIndex, Other, CopySize);
    Self->Private.BufferIndex += CopySize;
}

static void Array_AppendRaw(Array *Self, void *Other, uint32_t ByteSize)
{
    if(ByteSize % Self->Private.SizeofElement == 0)
        Array_Append(Self, Other, ByteSize / Self->Private.SizeofElement);
    else
        ERROR("Invalid 'ByteSize'! Must be multiple of 'SizeofElement'!");
}

static void Array_Compact(Array *Self)
{
    if(Array_GetSlackBytes(Self) > 0)
    {
        uint32_t RealSize = Array_Count(Self) * Self->Private.SizeofElement;

        // Strict fit
        Self->Private.Buffer = realloc(Self->Private.Buffer, RealSize);
        Self->Private.BufferSize = RealSize;
    }
}

static Array_ClassFunctions ClassFunctions = {
       .GetAt = Array_GetAt,
       .Add = Array_Add,
       .Append = Array_Append,
       .AppendRaw = Array_AppendRaw,
       .Remove = Array_Remove,
       .Count = Array_Count,
       .GetSlackElements = Array_GetSlackElements,
       .GetSlackBytes = Array_GetSlackBytes,
       .IsValidIndex = Array_IsValidIndex,
       .EnsureCapacity = Array_EnsureCapacity,
       .Compact = Array_Compact
};

static void Array_Init_INTERNAL(Array *Array)
{
    if(Array != NULL)
    {
        Array->Ft = &ClassFunctions;
    }
}

static void Array_Deinit_INTERNAL(Array *Array)
{
    if(Array != NULL)
    {
        Array->Ft = NULL;
    }
}

// Statically fct defined reader
void NewArrayStatic(Array *Array, uint32_t SizeofElement)
{
    Array_Init_INTERNAL(Array);
    Array_Ctor(Array, SizeofElement);
}

void DeleteArrayStatic(Array *Array)
{
    Array_Dtor(Array);
    Array_Deinit_INTERNAL(Array);
}

// Malloc created reader
Array *NewArray(uint32_t SizeofElement)
{
    Array *Array = malloc(sizeof(Array));
    NewArrayStatic(Array, SizeofElement);

    return Array;
}

void DeleteArray(Array *Array)
{
    DeleteArrayStatic(Array);
    free(Array);
}
