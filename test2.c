#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *_fgets(char *s, int n, FILE *iop);
int _fputs(char *, FILE *);
int _getline(char *, int);

int main(void)
{
    FILE *fp;
    char line[1000];

    if ((fp = fopen("lol.txt", "w")) == NULL)
        exit(EXIT_FAILURE);
    
    _fputs("its my func", fp);
    fclose(fp);
    if ((fp = fopen("lol.txt", "r")) == NULL)
        exit(EXIT_FAILURE);
    _fgets(line, 1000, fp);
    _fputs(line, stdout);
    return 0;
}

char *_fgets(char *s, int n, FILE *iop)
{
    register int c;
    register char *cs;

    cs = s;
    while (--n > 0 && (c = getc(iop)) != EOF)
        if ((*cs++ = c) == '\n')
            break;
    *cs = '\0';
    return (c == EOF && cs == s) ? NULL : s;
}

int _fputs(char *s, FILE *iop)
{
    int c;

    while ((c = *s++))
        putc(c, iop);
    return ferror(iop) ? EOF : 0;
}

int _getline(char *s, int max)
{
    if (fgets(s, max, stdin) == NULL)
        return 0;
    else
        return strlen(s);
}
