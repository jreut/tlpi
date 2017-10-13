#include <unistd.h>     /* Get NULL */
#include <string.h>     /* Get strcmp() */
#include <pwd.h>        /* Get getpwent() */

#include <stdio.h>
#include <stdlib.h>
#include "tlpi_hdr.h"

struct passwd *getpwnam(const char *login)
{
    struct passwd *pwd;
    while ((pwd = getpwent()) != NULL)
        if (strcmp(pwd->pw_name, login) == 0)
            break;
    endpwent();
    return pwd;
}

int main(int argc, char *argv[])
{
    int i;
    long lnmax;
    struct passwd *pwd;

    if (argc < 2)
        usageErr("%s username [...]\n", argv[0]);

    lnmax = sysconf(_SC_LOGIN_NAME_MAX);
    if (lnmax == -1)
        lnmax = 256;

    for (i = 1; i < argc; i++) {
        if (strlen(argv[i]) > (unsigned) lnmax)
            errExit("Argument %d is too long", i);

        pwd = getpwnam(argv[i]);

        if (pwd == NULL) {
            printf("%s was not found\n", argv[i]);
        } else {
            printf("%s has UID=%ld\n", argv[i], (long) pwd->pw_uid);
        }
    }

    exit(EXIT_SUCCESS);
}
