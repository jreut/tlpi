#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

extern int errno;

int dup(int oldfd) {
    return fcntl(oldfd, F_DUPFD, 0);
}

int dup2(int oldfd, int newfd) {
    if (fcntl(oldfd, F_GETFL) == -1) {
        errno = EBADF;
        return -1;
    }

    if (oldfd == newfd)
        return oldfd;

    close(newfd);

    return fcntl(oldfd, F_DUPFD, newfd);
}

int main(int argc, char **argv)
{
    return 0;
}
