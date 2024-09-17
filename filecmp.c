#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PROG    argv[0]
#define PATH1   argv[1]
#define PATH2   argv[2]
#define MAXLINE 1000

int main(int argc, char *argv[])
{
    FILE *ifp, *ofp;
    char line1[MAXLINE], line2[MAXLINE];

    if (argc != 3) {
        fprintf(stderr, "Usage: %s file1 file2\n", PROG);
        exit(EXIT_FAILURE);
    }
    if ((ifp = fopen(PATH1, "r")) == NULL) {
        perror(PATH1);
        exit(EXIT_FAILURE);
    }
    if ((ofp = fopen(PATH2, "r")) == NULL) {
        perror(PATH2);
        exit(EXIT_FAILURE);
    }

    while (fgets(line1, MAXLINE, ifp) != NULL && fgets(line2, MAXLINE, ofp) != NULL)
        if (strcmp(line1, line2) != 0) {
            printf("%-10s -> %s\n%-10s -> %s\n", PATH1, line1, PATH2, line2);
            goto final;
        }

    final:
    if (ferror(ifp) || ferror(ofp) || ferror(stdout)) {
        fprintf(stderr, "%s: error read file or write stdout\n", PROG);
        exit(EXIT_FAILURE);
    }
    return 0;
}
