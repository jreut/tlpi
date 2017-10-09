#include <fcntl.h>
#include "tlpi_hdr.h"

#ifndef BUF_SIZE
#define BUF_SIZE 1024
#endif

int main(int argc, char **argv)
{
        int sourceFd, destFd, openFlags, nulls;
        off_t offset;
        mode_t destPerms;
        ssize_t numRead;
        char buf[BUF_SIZE];

        if (argc != 3)
                usageErr("%s source dest\n", argv[0]);

        sourceFd = open(argv[1], O_RDONLY);
        if (sourceFd == -1)
                errExit("opening file %s", argv[1]);

        openFlags = O_WRONLY | O_CREAT | O_TRUNC;
        destPerms = S_IRUSR | S_IWUSR |
                S_IRGRP | S_IWGRP |
                S_IROTH | S_IWOTH;
        destFd = open(argv[2], openFlags, destPerms);
        if (destFd == -1)
                errExit("opening file %s", argv[2]);

        while ((numRead = read(sourceFd, buf, BUF_SIZE)) > 0) {
                nulls = 0;
                while (buf[nulls] == '\0')
                        nulls++;

                offset = lseek(destFd, nulls, SEEK_CUR);
                if (offset == -1)
                        errExit("seeking in %s", argv[2]);

                if (write(destFd, &buf[nulls], (numRead - nulls)) != (numRead - nulls))
                        fatal("couldn't write whole buffer");
        }

        close(sourceFd);
        close(destFd);

        exit(EXIT_SUCCESS);
}
