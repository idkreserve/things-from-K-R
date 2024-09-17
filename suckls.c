#include <stdio.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/dir.h>
#include <sys/stat.h>

#define MAX_PATH 1024

void fsize(char *);

int main(int argc, char *argv[])
{
    if (argc == 1)
        fsize(".");
    else
        while (--argc > 0)
            fsize(*++argv);
    return 0;
}

void dirwalk(char *dir, void (*fcn) (char *));

void fsize(char *name)
{
    struct stat stbuf;

    if (stat(name, &stbuf) == -1) {
        fprintf(stderr, "suckls: can't access %s\n", name);
        return;
    }
    if ((stbuf.st_mode & S_IFMT) == S_IFDIR)
        dirwalk(name, fsize);
    printf("%8ld %s\n", stbuf.st_size, name);
}

void dirwalk(char *dirname, void (*fcn) (char *))
{
    char name[MAX_PATH];
    DIR *dir;
    struct dirent *dirbuf;

    if ((dir = opendir(dirname)) == NULL) {
        fprintf(stderr, "dirwalk: can't open %s\n", dirname);
        return;
    }
    while ((dirbuf = readdir(dir)) != NULL) {
        if (strcmp(dirbuf->d_name, ".") == 0 || strcmp(dirbuf->d_name, "..") == 0)
            continue;
        if (strlen(dirname)+strlen(dirbuf->d_name)+2 > sizeof(name))
            fprintf(stderr, "dirwalk: name %s %s too long\n", dirname, dirbuf->d_name);
        else {
            sprintf(name, "%s/%s", dirname, dirbuf->d_name);
            (*fcn) (name);
        }
    }
    closedir(dir);
}
