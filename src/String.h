#ifndef __CU_STRING_H_
#define __CU_STRING_H_

#include "Array.h"

// Forward declaration
struct String;
typedef struct String String;

typedef struct String_ClassFunctions
{
    /**
     * @return  The character at the provided index.
     */
    int8_t (* const GetAt)(Array *Self, int32_t Index);

    /**
     * Appends an char to the string. Can re-allocate the internal array.
     */
    void (* const Add)(Array *Self, int8_t Char);

    /**
     * Appends the other chars to this array. Will use 'strlen' for the appending.
     */
    void (* const AppendString)(Array *Self, int8_t *String);

    /**
     * Appends an int32 to this string.
     */
    void (* const AppendInt32)(Array *Self, int32_t Value);

    /**
     * Append an uint32 to this string.
     */
    void (* const AppendUint32)(Array *Self, uint32_t Value);

    /**
     * Removes an element at the specified index. The whole array will be moved to fill in the gap.
     */
    void (* const Remove)(Array *Self, int32_t Index);

    /**
     * Removes a range of elements from the string, starting at the index of the length.
     *
     * Example: If string = "SomeWeirdName", RemoveRange(4, 5) will output "SomeName".
     */
    void (* const RemoveRange)(Array *Self, int32_t StartIndex, int32_t Length);

    /**
     * @return  The count of chars in the string (excluding the endline terminator).
     */
    uint32_t (* const Count)(Array *Self);

    /**
     * @return  The printf friendly version of this array.
     */
    int8_t (* const C_Str)(Array *Self);

    /**
     * @return  The count of unused array chars.
     */
    uint32_t (* const GetSlackBytes);

    /**
     * @return  True if the index is valid and usable, and false otherwise.
     */
    bool (* const IsValidIndex)(Array *Self, int32_t Index);

    /**
     * Ensures the inner size of the string buffer to hold.
     */
    void (* const EnsureCapacity)(Array *Self, uint32_t ExtraElementCount);

    /**
     * Compacts all slack.
     */
    void (* const Compact)(Array *Self);
} String_ClassFunctions;

/**
 * Private members, do not modify directly! Access only!
 */
typedef struct String_Private
{
    Array Data;
} String_Private;

/**
 * String class, useful for standard string operations that can grow/shrink without having
 * to face the C memcpy and realloc string management headache.
 *
 * Will map the elements to ASCII (8 bytes). Use other routines for utf8 or utf16.
 *
 * Quite memory heavy, don't over-use!
 */
typedef struct String
{
    // Don't modfy private! Access only!
    String_Private Private;

    // Do not tamper with this!
    String_ClassFunctions *Ft; // Function table
} String;

// Statically (stack) fct defined string
void NewStringStatic(String *String);
void NewStringStatic_1(String *String, int8_t *C_Str);
void DeleteStringStatic(String *String);

// Malloc created string
String *NewString();
String *NewString_1(int8_t *C_Str);
void DeleteString(String *String);

#define DELETE_STRING_SAFE(a) do { DeleteString(a); a = NULL; } while(0)

#endif /* __CU_STRING_H_ */
