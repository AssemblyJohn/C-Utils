#include "BufferedReader.h"

const uint32_t BufferedReader_DEFAULT_BUFFER_SIZE = 4096;

// INTERNAL FUNCTIONS

static void BufferedReader_Ctor(BufferedReader *Self)
{
    Self->Private.Descriptor = -1;
    Self->Private.CurrentOffset = 0;
    Self->Private.Buffer = malloc(BufferedReader_DEFAULT_BUFFER_SIZE);
    Self->Private.BufferSize = BufferedReader_DEFAULT_BUFFER_SIZE;
}

static void BufferedReader_Ctor_1(BufferedReader *Self, uint32_t BufferSize)
{
    Self->Private.Descriptor = -1;
    Self->Private.CurrentOffset = 0;
    Self->Private.Buffer = malloc(BufferSize);
    Self->Private.BufferSize = BufferSize;
}

static void BufferedReader_Dtor(BufferedReader *Self)
{
    if(Self->Private.Descriptor != -1)
        close(Self->Private.Descriptor);
    Self->Private.Descriptor= -1;

    if(Self->Private.Buffer != NULL)
        free(Self->Private.Buffer);
    Self->Private.Buffer = NULL;
}

static int BufferedReader_Open(BufferedReader *Self, const char *Path)
{
    int Fd = open(Path, O_RDONLY); // Readonly mode only

    if(Fd != -1)
    {
        Self->Private.Descriptor = Fd;
        Self->Private.CurrentOffset = 0;
    }

    return Fd;
}

static void BufferedReader_SetDescriptor(BufferedReader *Self, int Descriptor, bool CleanOld)
{
    if(CleanOld)
    {
        if(Self->Private.Descriptor != -1)
            close(Self->Private.Descriptor);
    }

    Self->Private.Descriptor = Descriptor;
    Self->Private.CurrentOffset = 0;
}

static ssize_t BufferedReader_Read(BufferedReader *Self, uint32_t Bytes)
{
    ssize_t Read = read(Self->Private.Descriptor, Self->Private.Buffer, Bytes);
    Self->Private.CurrentOffset += Read;

    return Read;
}

// Careful when using this stuff
static ssize_t BufferedReader_ReadRaw(BufferedReader *Self, uint8_t *Buff, uint32_t Bytes)
{
    ssize_t Read = read(Self->Private.Descriptor, Buff, Bytes);
    Self->Private.CurrentOffset += Read;

    return Read;
}

static int BufferedReader_ReadUint32(BufferedReader *Self, uint32_t *ReadValue)
{
    ssize_t Read = BufferedReader_Read(Self, 4);

    if(Read == sizeof(uint32_t))
    {
        *ReadValue = (Self->Private.Buffer[0] << 24) |
            (Self->Private.Buffer[1] << 16) |
            (Self->Private.Buffer[2] << 8) |
            (Self->Private.Buffer[3]);

        return 0;
    }

    return -1;
}

static int BufferedReader_ReadInt32(BufferedReader *Self, int32_t *ReadValue)
{
    ssize_t Read = BufferedReader_Read(Self, 4);

    if(Read == sizeof(uint32_t))
    {
        *ReadValue = (Self->Private.Buffer[0] << 24) |
            (Self->Private.Buffer[1] << 16) |
            (Self->Private.Buffer[2] << 8) |
            (Self->Private.Buffer[3]);

        return 0;
    }

    return -1;
}

int BufferedReader_ReadStringRawStatic(BufferedReader *Self, int8_t *StackBuff, uint32_t StackBuffSize)
{
    uint32_t StringSize;

    if(BufferedReader_ReadUint32(Self, &StringSize) == 0)
    {
        // It's bad, return
        if((StringSize + 1) > StackBuffSize)
            return -1;

        StackBuff[StringSize] = '\0';
        BufferedReader_ReadRaw(Self, StackBuff, StringSize);
    }

    return -1;
}

uint8_t * BufferedReader_ReadStringRawDynamic(BufferedReader *Self)
{
    uint32_t StringSize;

    // The string size excludes the endline character since we don't need it
    if(BufferedReader_ReadUint32(Self, &StringSize) == 0)
    {
        uint8_t *String = malloc(StringSize + 1);
        String[StringSize] = '\0'; // Set the endline

        BufferedReader_ReadRaw(Self, String, StringSize);

        return String;
    }

    return NULL;
}

off_t BufferedReader_Seek(BufferedReader *Self, int64_t Offset)
{
    if(Self->Private.Descriptor != -1)
    {
        off_t NewOffset = lseek(Self->Private.Descriptor, Offset, SEEK_SET);
        Self->Private.CurrentOffset = NewOffset;

        return NewOffset;
    }

    return -1;
}

int BufferedReader_Consume(BufferedReader *Self, ssize_t ReadSize, BufferedReader_OnConsume_Callback OnConsumeCallback, void *UserData)
{
    ssize_t ToConsume;

    if(ReadSize == 0)
        ToConsume = SSIZE_MAX; // We should hit the EOF in this case.
    else
        ToConsume = ReadSize;

    do {
        // If we have to consume more than the buffer size, it's fine, pass the buffer size, else consume less
        ssize_t ShouldRead = (ToConsume > Self->Private.BufferSize) ? Self->Private.BufferSize : ToConsume;

        ssize_t RealRead = BufferedReader_Read(Self, ShouldRead);

        // Break if we reached EOF prematurely, before consumed
        // bytes reached 0, in case the provided read > file size
        if(RealRead == 0)
            break;

        // If we have an error, return error
        if(RealRead == -1)
            return -1;

        OnConsumeCallback(Self->Private.Buffer, RealRead, UserData);

        // Sub the consumed bytes
        ToConsume -= RealRead;
    } while(ToConsume > 0); // While we still have data to consume

    return 0;
}

static BufferedReader_ClassFunctions ClassFunctions = {
    .Open = BufferedReader_Open,
    .SetDescriptor = BufferedReader_SetDescriptor,
    .Read = BufferedReader_Read,
    .ReadUint32 = BufferedReader_ReadUint32,
    .ReadInt32 = BufferedReader_ReadInt32,
    .ReadStringRawStatic = BufferedReader_ReadStringRawStatic,
    .ReadStringRawDynamic = BufferedReader_ReadStringRawDynamic,
    .Seek = BufferedReader_Seek,
    .Consume = BufferedReader_Consume
};

static void BufferedReader_Init_INTERNAL(BufferedReader *Reader)
{
    // Internal fct table & self init
    if(Reader != NULL)
    {
        Reader->Ft = &ClassFunctions;
        // Reader->Self = Reader;
    }
}

static void BufferedReader_Deinit_INTERNAL(BufferedReader *Reader)
{
    if(Reader != NULL)
    {
        Reader->Ft = NULL;
        // Reader->Self = NULL;
    }
}

// Statically fct defined reader
void NewBufferedReaderStatic(BufferedReader *Reader)
{
    BufferedReader_Init_INTERNAL(Reader);
    BufferedReader_Ctor(Reader);
}

void DeleteBufferedReaderStatic(BufferedReader *Reader)
{
    BufferedReader_Dtor(Reader);
    BufferedReader_Deinit_INTERNAL(Reader);
}

// Malloc created reader
BufferedReader *NewBufferedReader(void)
{
    BufferedReader *Reader = malloc(sizeof(BufferedReader));
    NewBufferedReaderStatic(Reader);

    return Reader;
}

void DeleteBufferedReader(BufferedReader *Reader)
{
    DeleteBufferedReaderStatic(Reader);
    free(Reader);
}
