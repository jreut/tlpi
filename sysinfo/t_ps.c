#include <limits.h>
#include <dirent.h>
#include <ctype.h>
#include "tlpi_hdr.h"
#include "ugid_functions.h"

#define	MAX_LINE	1000

int main(int argc, char ** argv)
{
	uid_t givenUid, fileUid;
	DIR *dirp;
	struct dirent *direntp;
	char path[PATH_MAX];
	FILE *fp;
	char line[MAX_LINE];
	char *command;
	Boolean gotUid, gotName;

	if (argc != 2)
		usageErr("%s username", argv[0]);

	givenUid = userIdFromName(argv[1]);

	dirp = opendir("/proc");
	if (dirp == NULL)
		errExit("opendir /proc");

	for (;;) {
		errno = 0;
		direntp = readdir(dirp);
		if (direntp == NULL) {
			if (errno != 0)
				errExit("readdir /proc");
			else
				break; /* end of directory */
		}

		/* we only care about directories for PIDs */ 
		if (direntp->d_type != DT_DIR || !isdigit(direntp->d_name[0]))
			continue;

		snprintf(path, PATH_MAX, "/proc/%s/status", direntp->d_name);

		fp = fopen(path, "r");
		if (fp == NULL)
			/* ignore errors because the process may have just
			 * terminated */
			continue;

		gotUid = FALSE;
		gotName = FALSE;
		while (!gotUid || !gotName) {
			if (fgets(line, MAX_LINE, fp) == NULL)
				break; /* error or EOF, just move on */

			if (strncmp(line, "Name:", 5) == 0) {
				command = strdup(line + 5);
				if (command == NULL)
					errExit("strdup Name");

				gotName = TRUE;
			}

			if (strncmp(line, "Uid:", 4) == 0) {
				/* get the real user id (which is the first of
				 * the tab-delimited ids */
				fileUid = strtol(line + 4, NULL, 10);
				gotUid = TRUE;
			}

		}

		fclose(fp);

		if (gotName && gotUid && fileUid == givenUid)
			printf("%5s %s", direntp->d_name, command + 1);
	}

	closedir(dirp);

	exit(EXIT_SUCCESS);
}
