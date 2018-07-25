
#ifndef __CU_BUFFEREDWRITTER_H_
#define __CU_BUFFEREDWRITTER_H_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>

extern const uint32_t BufferedWritter_DEFAULT_BUFFER_SIZE;

// Forward declaration
struct BufferedWritter;
typedef struct BufferedWritter BufferedWritter;

typedef struct BufferedWritter_ClassFunctions
{
    /**
     * If we don't use 'SetDescriptor'  then we can use the 'Open' routine.
     */
    int (* const Open)(BufferedWritter *Self, const char *Path);

    /**
     * Sets the new file descriptor to read from. Will close any other pending descriptor
     * if the 'CleanOld' is set. The current offset will be reseted too.
     */
    void (* const SetDescriptor)(BufferedWritter *Self, int Descriptor, bool CleanOld);

    /**
     * Offsets the read, from the beginning of the descriptor.
     */
    off_t (* const Seek)(BufferedWritter *Self, int64_t Offset);

    /**
     * Reads bytes. The read bytes must not be larger than the size of the internal buffer.
     *
     * @return Same as 'read'. -1 on error, 0 on EOF, else number of bytes read
     */
    ssize_t (* const Write)(BufferedWritter *Self, uint8_t *Buffer, uint32_t Bytes);

    /**
     * Reads a uint32_t value.
     *
     * @return 0 on success, != 0 failure
     */
    int (* const WriteUint32)(BufferedWritter *Self, uint32_t WriteValue);

    /**
     * Reads a uint32_t value.
     *
     * @return 0 on success, != 0 failure
     */
    int (* const WriteInt32)(BufferedWritter *Self, int32_t WriteValue);

    int (* const WriteStringRaw)(BufferedWritter *Self, int8_t *String);

    /**
     * Flush all virtual writes to the descriptor.
     */
    void (* const Flush)(BufferedWritter *Self);

} BufferedWritter_ClassFunctions;

/**
 * Public members, safe to access directly.
 */
//typedef struct BufferedWritter_Public
//{
//};

/**
 * Private members, do not modify directly! Access only!
 */
typedef struct BufferedWritter_Private
{
    int Descriptor;
    off_t CurrentOffset;

    uint8_t *Buffer;
    size_t BufferSize;
} BufferedWritter_Private;

typedef struct BufferedWritter
{
    // Access public anytime!
    // BufferedReader_Public Public;

    // Don't access private!
    BufferedWritter_Private Private;

    // Do not tamper with these!
    BufferedWritter_ClassFunctions *Ft; // Function table
} BufferedWritter;

// Statically (stack) fct defined reader
void NewBufferedWritterStatic(BufferedWritter *Reader);
void DeleteBufferedWritterStatic(BufferedWritter *Reader);

// Malloc created reader
BufferedWritter *NewBufferedWritter(void);
void DeleteBufferedWritter(BufferedWritter *Reader);

#define DELETE_BUFFERED_WRITTER_SAFE(a) do { DeleteBufferedWritter(a); a = NULL; } while(0)

#endif /* __CU_BUFFEREDWRITTER_H_ */
