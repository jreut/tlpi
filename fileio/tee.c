#include <fcntl.h>
#include "tlpi_hdr.h"

#define MAX_READ 20

int main(int argc, char* argv[])
{
	char *progname = argv[0];
	char buffer[MAX_READ + 1];
	char *filename;
	int fd, openFlags;
	ssize_t numRead, numWritten;
	char optionChar;
	Boolean append = FALSE;

	while ((optionChar = getopt(argc, argv, "a")) != -1) {
		switch(optionChar) {
			case 'a':
				append = TRUE;
				break;
			case '?':
			default:
				usageErr("%s [-a] file\n", progname);
		}

	}
	argc -= optind;
	argv += optind;

	if (argc < 1)
		usageErr("%s [-a] file\n", progname);

	filename = argv[0];

	if (append)
		openFlags = O_WRONLY | O_CREAT | O_APPEND;
	else
		openFlags = O_WRONLY | O_CREAT | O_TRUNC;

	fd = open(filename, openFlags,
			S_IRUSR | S_IWUSR |
			S_IRGRP | S_IWGRP |
			S_IROTH | S_IWOTH); /* rw-rw-rw- */

	if (fd == -1)
		errExit("open");

	while ((numRead = read(STDIN_FILENO, buffer, MAX_READ)) != 0) {
		if (numRead == -1) {
			close(fd);
			errExit("read");
		}

		buffer[numRead] = '\0';

		numWritten = write(fd, buffer, numRead);
		if (numWritten == -1) {
			close(fd);
			errExit("write to file");
		}

		numWritten = write(STDOUT_FILENO, buffer, numRead);
		if (numWritten == -1) {
			close(fd);
			errExit("write to stdout");
		}
	}

	close(fd);

	return 0;
}
