
#include "BufferedWritter.h"

const uint32_t BufferedWritter_DEFAULT_BUFFER_SIZE = 4096;

// INTERNAL FUNCTIONS

static void BufferedWritter_Ctor(BufferedWritter *Self)
{
    Self->Private.Descriptor = -1;
    Self->Private.CurrentOffset = 0;
    Self->Private.Buffer = malloc(BufferedWritter_DEFAULT_BUFFER_SIZE);
    Self->Private.BufferSize = BufferedWritter_DEFAULT_BUFFER_SIZE;
}

static void BufferedWritter_Ctor_1(BufferedWritter *Self, uint32_t BufferSize)
{
    Self->Private.Descriptor = -1;
    Self->Private.CurrentOffset = 0;
    Self->Private.Buffer = malloc(BufferSize);
    Self->Private.BufferSize = BufferSize;
}

static void BufferedWritter_Dtor(BufferedWritter *Self)
{
    // Flush on dtor just in case we forgot
    Self->Ft->Flush(Self);

    if(Self->Private.Descriptor != -1)
        close(Self->Private.Descriptor);
    Self->Private.Descriptor= -1;

    if(Self->Private.Buffer != NULL)
        free(Self->Private.Buffer);
    Self->Private.Buffer = NULL;
}

static int BufferedWritter_Open(BufferedWritter *Self, const char *Path)
{
    int Fd = open(Path, O_WRONLY); // Readonly mode only

    if(Fd != -1)
    {
        Self->Private.Descriptor = Fd;
        Self->Private.CurrentOffset = 0;
    }

    return Fd;
}

static void BufferedWritter_SetDescriptor(BufferedWritter *Self, int Descriptor, bool CleanOld)
{
    if(CleanOld)
    {
        if(Self->Private.Descriptor != -1)
            close(Self->Private.Descriptor);
    }

    Self->Private.Descriptor = Descriptor;
    Self->Private.CurrentOffset = 0;
}

off_t BufferedWritter_Seek(BufferedWritter *Self, int64_t Offset)
{
    if(Self->Private.Descriptor != -1)
    {
        off_t NewOffset = lseek(Self->Private.Descriptor, Offset, SEEK_SET);
        Self->Private.CurrentOffset = NewOffset;

        return NewOffset;
    }

    return -1;
}

static ssize_t BufferedWritter_Write(BufferedWritter *Self, uint8_t *Buffer, uint32_t Bytes)
{
    // TODO: Attempt to fill our inner buffer

    ssize_t Wrote = write(Self->Private.Descriptor, Buffer, Bytes);
    Self->Private.CurrentOffset += Wrote;

    return Wrote;
}

static int BufferedWritter_WriteUint32(BufferedWritter *Self, uint32_t WriteValue)
{
    uint8_t Buff[sizeof(uint32_t)];

    Buff[0] = (int)((WriteValue >> 24) & 0xFF);
    Buff[1] = (int)((WriteValue >> 16) & 0xFF);
    Buff[2] = (int)((WriteValue >> 8) & 0XFF);
    Buff[3] = (int)((WriteValue & 0XFF));

    ssize_t Written = BufferedWritter_Write(Self, Buff, sizeof(uint32_t));

    if(Written == sizeof(uint32_t))
        return 0;

    return -1;
}

static int BufferedWritter_WriteInt32(BufferedWritter *Self, int32_t WriteValue)
{
    uint8_t Buff[sizeof(uint32_t)];

    Buff[0] = (int)((WriteValue >> 24) & 0xFF);
    Buff[1] = (int)((WriteValue >> 16) & 0xFF);
    Buff[2] = (int)((WriteValue >> 8) & 0XFF);
    Buff[3] = (int)((WriteValue & 0XFF));

    ssize_t Written = BufferedWritter_Write(Self, Buff, sizeof(uint32_t));

    if(Written == sizeof(uint32_t))
        return 0;

    return -1;
}

int BufferedWritter_WriteStringRaw(BufferedWritter *Self, uint8_t *String)
{
    ssize_t Length = strlen(String);

    if(BufferedWritter_WriteUint32(Self, Length) == 0)
    {
        // Does not write the '\0' endline, that's appended when reading
        ssize_t Written = BufferedWritter_Write(Self, String, Length);

        if(Written == Length)
            return 0;
    }

    return -1;
}

static void BufferedWritter_Flush(BufferedWritter *Self)
{
    // TODO: When we'll have the buffered writter with the internal buffer
    // BufferedWritter_Write(Self, Self->Private.Buffer, Self->Private.BufferOffset);
}

static BufferedWritter_ClassFunctions ClassFunctions = {
    .Open = BufferedWritter_Open,
    .SetDescriptor = BufferedWritter_SetDescriptor,
    .Write = BufferedWritter_Write,
    .WriteUint32 = BufferedWritter_WriteUint32,
    .WriteInt32 = BufferedWritter_WriteInt32,
    .WriteStringRaw = BufferedWritter_WriteStringRaw,
    .Seek = BufferedWritter_Seek,
    .Flush = BufferedWritter_Flush,
};

static void BufferedWritter_Init_INTERNAL(BufferedWritter *Reader)
{
    // Internal fct table & self init
    if(Reader != NULL)
    {
        Reader->Ft = &ClassFunctions;
        // Reader->Self = Reader;
    }
}

static void BufferedWritter_Deinit_INTERNAL(BufferedWritter *Reader)
{
    if(Reader != NULL)
    {
        Reader->Ft = NULL;
        // Reader->Self = NULL;
    }
}

// Statically fct defined reader
void NewBufferedWritterStatic(BufferedWritter *Reader)
{
    BufferedWritter_Init_INTERNAL(Reader);
    BufferedWritter_Ctor(Reader);
}

void DeleteBufferedWritterStatic(BufferedWritter *Reader)
{
    BufferedWritter_Dtor(Reader);
    BufferedWritter_Deinit_INTERNAL(Reader);
}

// Malloc created reader
BufferedWritter *NewBufferedWritter(void)
{
    BufferedWritter *Reader = malloc(sizeof(BufferedWritter));
    NewBufferedWritterStatic(Reader);

    return Reader;
}

void DeleteBufferedWritter(BufferedWritter *Reader)
{
    DeleteBufferedWritterStatic(Reader);
    free(Reader);
}
