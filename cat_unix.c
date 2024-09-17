#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdnoreturn.h>

noreturn void panic(char *, ...);

int main(int argc, char *argv[])
{
    int fd;
    void filecopy(int, int);

    if (argc == 1) {
        filecopy(0, 1);
    } else
        while (--argc > 0)
            if ((fd = open(*++argv, O_RDONLY)) == -1)
                panic("cat: can't open file %s\n", *argv);
            else {
                filecopy(fd, 1);
                close(fd);
            }
    if (ferror(stdout)) {
        fprintf(stderr, "%s: error writing stdout\n", argv[0]);
        exit(2);
    }
    return 0;
}

void filecopy(int fd1, int fd2)
{
    int n;
    char c;

    while ((n = read(fd1, &c, 1)) > 0)
        if (write(fd2, &c, 1) != n)
            panic("cat: can't write to file by descriptor %d\n", fd2);
}

noreturn void panic(char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    fprintf(stderr, "error: ");
    vfprintf(stderr, fmt, ap);
    putc('\n', stderr);
    va_end(ap);
    exit(EXIT_FAILURE);
}
