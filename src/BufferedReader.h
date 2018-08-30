
#ifndef __CU_BUFFEREDREADER_H_
#define __CU_BUFFEREDREADER_H_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>

extern const uint32_t BufferedReader_DEFAULT_BUFFER_SIZE;

/**
 * Consume function.
 *
 * @param First param is the buffer
 * @param Second param is the buffer size
 * @param Third param is some user data
 */
typedef void (* BufferedReader_OnConsume_Callback)(uint8_t *, ssize_t, void *);

// Forward declaration
struct BufferedReader;
typedef struct BufferedReader BufferedReader;

typedef struct BufferedReader_ClassFunctions
{
    /**
     * If we don't use 'SetDescriptor'  then we can use the 'Open' routine.
     */
    int (* const Open)(BufferedReader *Self, const char *Path);
    /**
     * Sets the new file descriptor to read from. Will close any other pending descriptor.
     * The current offset will be reseted too.
     */
    void (* const SetDescriptor)(BufferedReader *Self, int Descriptor, bool CleanOld);
    /**
     * Reads bytes. The read bytes must not be larger than the size of the internal buffer.
     *
     * @return Same as 'read'. -1 on error, 0 on EOF, else number of bytes read
     */
    ssize_t (* const Read)(BufferedReader *Self, uint32_t Bytes);

    /**
     * Reads a uint32_t value.
     *
     * @return 0 on success, != 0 failure
     */
    int (* const ReadUint32)(BufferedReader *Self, uint32_t *ReadValue);

    /**
     * Reads a uint32_t value.
     *
     * @return 0 on success, != 0 failure
     */
    int (* const ReadInt32)(BufferedReader *Self, int32_t *ReadValue);

    /**
     * Reads a string that was written with a 'BufferedWritter' of the form uint32_t strlen + the string.
     *
     * @param StackBuff Stack allocated buffer. Be sure that you know what you've written. If the string size > size of the StackBuff
     *                  it will not do anything & it will return. Be very sure when using this
     */
    int (* const ReadStringRawStatic)(BufferedReader *Self, int8_t *StackBuff, uint32_t StackBuffSize);

    /**
     * Reads a string that was written with a 'BufferedWritter' of the form uint32_t (strlen) + the string. The
     * returned buffer must be allocated using 'free'.
     *
     * @return  The heap allocated string pointer or null if there was an error during read
     */
    int8_t * (* const ReadStringRawDynamic)(BufferedReader *Self);

    /**
     * Offsets the read, from the beginning of the descriptor.
     */
    off_t (* const Seek)(BufferedReader *Self, int64_t Offset);

    /**
     * Consumes the specified size from the descriptor, starting at the current offset.
     *
     * @param ReadSize  If 0, will read the whole descriptor
     */
    int (* const Consume)(BufferedReader *Self, ssize_t ReadSize, BufferedReader_OnConsume_Callback OnConsumeCallback, void *UserData);
} BufferedReader_ClassFunctions;

/**
 * Public members, safe to access directly.
 */
//typedef struct BufferedReader_Public
//{
//};

/**
 * Private members, do not modify directly! Access only!
 */
typedef struct BufferedReader_Private
{
    int Descriptor;
    off_t CurrentOffset;

    uint8_t *Buffer;
    size_t BufferSize;
} BufferedReader_Private;

typedef struct BufferedReader
{
    // Access public anytime!
    // BufferedReader_Public Public;

    // Don't access private!
    BufferedReader_Private Private;

    // Do not tamper with these!
    BufferedReader_ClassFunctions *Ft; // Function table
} BufferedReader;

// Statically (stack) fct defined reader
void NewBufferedReaderStatic(BufferedReader *Reader);
void NewBufferedReaderStatic_1(BufferedReader *Reader, uint32_t BufferSize);
void DeleteBufferedReaderStatic(BufferedReader *Reader);

// Malloc created reader
BufferedReader *NewBufferedReader(void);
void DeleteBufferedReader(BufferedReader *Reader);

#define DELETE_BUFFERED_READER_SAFE(a) do { DeleteBufferedReader(a); a = NULL; } while(0)

/*
#define BUFFERED_READER_SAFE_BLOCK(code_block) do { \
    BufferedReader buffered_reader_name; \
    NewBufferedReaderStatic(&buffered_reader_name); \
    code_block; \
    DeleteBufferedReaderStatic(&buffered_reader_name); \
    } \
    while(0) \
*/

#endif /* BUFFEREDREADER_H_ */
