#include <stdio.h>
#include <stdlib.h>

#define PROG argv[0]

static size_t page = 1;

int main(int argc, char *argv[])
{
    FILE *fp;
    int c;

    if (argc == 1) {
        perror(PROG);
        exit(EXIT_FAILURE);
    } else
        while (--argc > 0)
            if ((fp = fopen(*++argv, "r")) == NULL) {
                fprintf(stderr, "%s: can't open %s\n", PROG, *argv);
                exit(EXIT_FAILURE);
            } else {
                for (c = 0; c < 20; c++)
                    putchar('#');
                printf("%s", *argv);
                for (c = 0; c < 20; c++)
                    putchar('#');
                putchar('\n');

                while ((c = fgetc(fp)) != EOF)
                    putchar(c);
                
                for (c = 0; c < 50; c++)
                    putchar('-');
                printf("%ld", page++);
                for (c = 0; c < 10; c++)
                    putchar('-');
                putchar('\n');

                fclose(fp);
            }
    return 0;
}
