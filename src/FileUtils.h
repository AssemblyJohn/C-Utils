
#ifndef __CU_FILEUTILS_H_
#define __CU_FILEUTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

off_t FileUtilsGetFileSize(const char* Path)
{
    off_t size = -1;
    int fd = open(Path, O_RDONLY);

    if (fd != -1) {
        size = lseek(fd, 0, SEEK_END);
        close(fd);
    }

    return size;
}

#endif /* __CU_FILEUTILS_H_ */
