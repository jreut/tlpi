#include <fcntl.h>
#include <unistd.h>

int main()
{
    char file[] = "a file";
    int fd1, fd2, fd3;

    fd1 = open(file, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    fd2 = dup(fd1);
    fd3 = open(file, O_RDWR);
    write(fd1, "Hello,", 6);
    // Hello,
    write(fd2, " world", 6);
    // Hello, world
    lseek(fd2, 0, SEEK_SET);
    write(fd1, "HELLO,", 6);
    // HELLO, world
    write(fd3, "Gidday", 6);
    // Gidday world
}
