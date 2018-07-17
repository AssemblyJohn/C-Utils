
#include <stdio.h>
#include <stdlib.h>

#include <stdint.h>

#include "BufferedReader.h"
#include "BufferedWritter.h"
#include "Array.h"

// typedef int (*__compar_fn_t) (const void *, const void *);

void TestReads(int TempFd)
{
    BufferedReader BuffReader;
    NewBufferedReaderStatic(&BuffReader);

    BuffReader.Ft->SetDescriptor(&BuffReader, TempFd, false);

    // Read the values
    int32_t ReadValue = 0;

    BuffReader.Ft->ReadInt32(&BuffReader, &ReadValue);
    printf("Read: %d\n", ReadValue);
    BuffReader.Ft->ReadInt32(&BuffReader, &ReadValue);
    printf("Read: %d\n", ReadValue);
    BuffReader.Ft->ReadInt32(&BuffReader, &ReadValue);
    printf("Read: %d\n", ReadValue);
    BuffReader.Ft->ReadInt32(&BuffReader, &ReadValue);
    printf("Read: %d\n", ReadValue);

    uint32_t ReadValueU = 0;

    BuffReader.Ft->ReadUint32(&BuffReader, &ReadValueU);
    printf("Read: %u\n", ReadValueU);
    BuffReader.Ft->ReadUint32(&BuffReader, &ReadValueU);
    printf("Read: %u\n", ReadValueU);
    BuffReader.Ft->ReadUint32(&BuffReader, &ReadValueU);
    printf("Read: %u\n", ReadValueU);
    BuffReader.Ft->ReadUint32(&BuffReader, &ReadValueU);
    printf("Read: %u\n", ReadValueU);

    uint8_t Buff[100];
    BuffReader.Ft->ReadStringRawStatic(&BuffReader, Buff, 100);

    printf("Read string: %s\n", Buff);

    DeleteBufferedReaderStatic(&BuffReader);
}

void TestWrites(int TempFd)
{
    BufferedWritter BuffWritter;
    NewBufferedWritterStatic(&BuffWritter);

    BuffWritter.Ft->SetDescriptor(&BuffWritter, TempFd, false);

    BuffWritter.Ft->WriteInt32(&BuffWritter, INT_MAX);
    BuffWritter.Ft->WriteInt32(&BuffWritter, INT_MIN);
    BuffWritter.Ft->WriteInt32(&BuffWritter, -1234);
    BuffWritter.Ft->WriteInt32(&BuffWritter, 1234);

    BuffWritter.Ft->WriteUint32(&BuffWritter, UINT32_MAX);
    BuffWritter.Ft->WriteUint32(&BuffWritter, 0);
    BuffWritter.Ft->WriteUint32(&BuffWritter, 4321);
    BuffWritter.Ft->WriteUint32(&BuffWritter, 1234);

    BuffWritter.Ft->WriteStringRaw(&BuffWritter, "Some String Written");

    BuffWritter.Ft->Seek(&BuffWritter, 0);
    BuffWritter.Ft->SetDescriptor(&BuffWritter, -1, false);

    DeleteBufferedWritterStatic(&BuffWritter);
}

struct FillerBuffer
{
    uint8_t Chunk[4096];
    uint8_t Checksum[8];
};

void OnConsume(uint8_t *Buff, ssize_t BuffSize, void *UserData)
{
    Array *Arr = (Array *)UserData;
    Arr->Ft->AppendRaw(Arr, Buff, BuffSize);
}

int main(void)
{
    // Test out read/write
    char template[] = "/tmp/file_XXXXXX";
    int TempFd = mkstemp(template);

    // Let's test it...
    // TestWrites(TempFd);
    // TestReads(TempFd);

    // "/etc/init.d/isc-dhcp-server"
    BufferedReader BuffReader;
    NewBufferedReaderStatic(&BuffReader);
    BuffReader.Ft->Open(&BuffReader, "/etc/init.d/isc-dhcp-server");

    Array BufferArray;
    NewArrayStatic(&BufferArray, 1); // Byte array

    BuffReader.Ft->Consume(&BuffReader, 0, OnConsume, &BufferArray);
    BufferArray.Ft->Compact(&BufferArray);

    printf("Read from array: %s\n");

    DeleteArrayStatic(&BufferArray);
    DeleteBufferedReaderStatic(&BuffReader);

    printf("End...\n");

    return EXIT_SUCCESS;
}
