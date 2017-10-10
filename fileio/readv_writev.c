#include <unistd.h>
#include <stdlib.h>
#include <sys/uio.h>
#include "tlpi_hdr.h"

ssize_t writev(int fd, const struct iovec *iov, int iovcnt)
{
    int i;
    ssize_t numWritten, totalWritten;

    totalWritten = 0;

    for (i = 0; i < iovcnt; i++)
    {
        numWritten = write(fd, iov[i].iov_base, iov[i].iov_len);
        if (numWritten == -1)
            return -1;
        totalWritten += numWritten;
    }

    return totalWritten;
}

ssize_t readv(int fd, const struct iovec *iov, int iovcnt)
{
    int i;
    ssize_t numRead, totalRead;

    totalRead = 0;

    for (i = 0; i < iovcnt; i++)
    {
        numRead = read(fd, iov[i].iov_base, iov[i].iov_len);
        if (numRead == -1)
            return -1;
        totalRead += numRead;
        if (numRead == 0)
            return totalRead;
    }

    return totalRead;
}

int main()
{
}
