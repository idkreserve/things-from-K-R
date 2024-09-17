#include <stdio.h>
#include <stdlib.h>

#define TAB_WIDTH 8

int main(int argc, char *argv[])
{
    int start_with = 0, every = 0;
    int i, c, position = 1;
    int tabs, spaces;

    for (i = 1; i < argc; i++)
        switch ((*++argv)[0]) {
            case '-':
                start_with = atoi(++argv[0]);
                break;
            case '+':
                every = atoi(++argv[0]);
                break;
        }

    while ((c = getchar()) != EOF) {
        for (
            tabs = spaces = 0;
            c == ' ' && ((start_with == 0 || position >= start_with) && (every == 0 || position % every == 0));
            spaces++, c = getchar(), start_with = 0
        )
            ;
        for (i = spaces; i - TAB_WIDTH >= 0; i -= TAB_WIDTH, tabs++)
            ;
        spaces = spaces - tabs * TAB_WIDTH;
        // printf("!!!!!!!! %c -> start_with=%d, every=%d, position=%d, tabs=%d, spaces=%d, position %% every=%d, equals=%d\n", c, start_with, every, position, tabs, spaces, position % every, c == ' ' && (every == 0 || position % every == 0));
        // printf("%c -> tabs=%d, spaces=%d, position=%d\n", c, tabs, spaces, position);   
        while (tabs-- > 0)
            putchar('\t');
        while (spaces-- > 0)
            putchar('_');
        
        putchar(c);
        position++;
    }
    return 0;
}
