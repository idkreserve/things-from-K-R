#include <stdio.h>
#include <stdlib.h>

#define TAB_WIDTH 8

int main(int argc, char *argv[])
{
    int start_with = 0, every = 0;
    int i, c, position = 1;

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
        if (c == '\t' && ((start_with == 0 || position >= start_with) && (every == 0 || position % every == 0))) {
            for (i = 0, start_with = 0; i < TAB_WIDTH; i++)
                putchar(' ');
        }
        else
            putchar(c);
        position++;
    }
    return 0;
}
