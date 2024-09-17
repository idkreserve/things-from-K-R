#include <stdio.h>
#include <ctype.h>
#include <string.h>

enum { LOWER, UPPER };

int main(int argc, char *argv[])
{
    int (*fn) (int);
    int i, letter_case = LOWER;

    for (i = strlen(*argv)-1; isalpha((*argv)[i]); i--) {
        if (islower((*argv)[i])) {
            fn = tolower;
            letter_case = LOWER;
            break;
        }
        fn = toupper;
        letter_case = UPPER;
    }
    while ((i = getchar()) != EOF)
        putchar(fn(i));

    return 0;
}
