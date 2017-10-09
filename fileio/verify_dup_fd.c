#include <fcntl.h>
#include "tlpi_hdr.h"

int main(int argc, char *argv[])
{
    int flags, fd1, fd2, flags1, flags2;
    off_t offset1, offset2;

    if (argc < 2)
        usageErr("%s file", argv[0]);

    flags = O_RDONLY | O_EXCL;
    fd1 = open(argv[1], flags);
    if(fd1 == -1)
        errExit("open");

    fd2 = dup(fd1);
    if (fd2 == -1)
        errExit("dup");

    offset1 = lseek(fd1, 200, SEEK_SET);
    if (offset1 == -1)
        errExit("seek");

    offset2 = lseek(fd2, 0, SEEK_CUR);
    if (offset2 == -1)
        errExit("seek");

    flags1 = fcntl(fd1, F_GETFL);
    flags2 = fcntl(fd2, F_GETFL);

    printf("FD1\t(%d)\t%X\t%lld\n", fd1, flags1, (long long) offset1);
    printf("FD2\t(%d)\t%X\t%lld\n", fd2, flags2, (long long) offset2);

    close(fd1);
    close(fd2);

    exit(EXIT_SUCCESS);
}
