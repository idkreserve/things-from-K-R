#include <stdio.h>

#define UNPRINTED   32
#define MAXLINE     50

int main(void)
{
    int c, nchar;

    nchar = 1;
    while ((c = getchar()) != EOF) {
        printf((c <= UNPRINTED && c != '\n') ? "0x%x" : "%c", c);
        
        if (nchar == MAXLINE) {
            putchar('\n');
            nchar = 1;
        } else
            if (c != '\n')
                nchar++;
    }
    return 0;
}
