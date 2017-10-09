#include <fcntl.h>
#include "tlpi_hdr.h"

int main(int argc, char *argv[])
{
#define BUF_SIZE 1024
    int fd;
    ssize_t numRead, numWritten;
    char buf[BUF_SIZE];

    if (argc < 2)
        usageErr("%s file", argv[0]);

    fd = open(argv[1], O_APPEND | O_WRONLY);
    if (fd == -1)
        errExit("open");

    if (lseek(fd, 0, SEEK_SET) != 0)
        errExit("lseek");

    while ((numRead = read(STDIN_FILENO, buf, BUF_SIZE)) != 0) {
        if (numRead == -1)
            errExit("read");

        numWritten = write(fd, buf, numRead);
        if(numWritten != numRead)
            fatal("bad/partial write");
    }

    close(fd);

    exit(EXIT_SUCCESS);
}
