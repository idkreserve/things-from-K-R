#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAXLINES 5000
char *lineptr[MAXLINES];

uint8_t numeric     =   0;  /* 1, если числовая сортировка */
uint8_t reversed    =   0;  /* 1, если сортировка в обратном порядке */
uint8_t ignore_case =   0;  /* 1, если нужно игнорировать регистр при сортировке */
uint8_t dir_order   =   0;  /* 1, если сравнение выполняется только по буквам, цифрам, и пробелам */

int readlines(char *lineptr[], int nlines);
void writelines(char *lineptr[], int nlines);

void _qsort(void *lineptr[], int left, int right,
    int (*comp) (void *, void *));
int numcmp(const char *, const char *);
int dstrcmp(const char *, const char *);

int main(int argc, char *argv[])
{
    int nlines, c;
    
    while (--argc > 0 && (*++argv)[0] == '-')
        while ((c = *++argv[0]))
            switch (c) {
                case 'n':
                    numeric = 1;
                    break;
                case 'r':
                    reversed = 1;
                    break;
                case 'f':
                    ignore_case = 1;
                    break;
                case 'd':
                    dir_order = 1;
                    break;
                default:
                    fprintf(stderr, "Illegal option %c\n", c);
                    return EXIT_FAILURE;
                    break;
            }
    if ((nlines = readlines(lineptr, MAXLINES)) >= 0) {
        _qsort((void **) lineptr, 0, nlines-1,
            (int (*) (void*,void*)) (numeric ? numcmp : dstrcmp));
        writelines(lineptr, nlines);
        return 0;
    } else {
        fprintf(stderr, "Input too big to sort");
        return 1;
    }
    return 0;
}

void _qsort(void *v[], int left, int right,
    int (*comp) (void *, void *))
{
    int i, last;
    
    void swap(void **v, const int i, const int j);
    void strlow(char *s);

    if (left >= right)
        return;
    swap(v, left, (left + right)/2);
    last = left;
    for (i = left+1; i <= right; i++) {
        char *s1 = malloc(sizeof(v+i));
        char *s2 = malloc(sizeof(v+left));

        strcpy(s1, v[i]);
        strcpy(s2, v[left]);
        if (ignore_case) {
            strlow(s1);
            strlow(s2);
        }
        if (((*comp) (s1, s2) < 0) != reversed)
            swap(v, ++last, i);
    }   
    swap(v, left, last);
    _qsort(v, left, last-1, comp);
    _qsort(v, last+1, right, comp);
}

int dstrcmp(const char *s, const char *t)
{
    #define MAXLEN 1000
    if (!dir_order)
        return strcmp(s, t);

    char s1[MAXLEN], s2[MAXLEN];
    while (!isalnum(*s) && !isdigit(*s) && *s != '\0')
        s++;
    while (!isalnum(*t) && !isdigit(*t) && *t != '\0')
        t++;

    strcpy(s1, s);
    strcpy(s2, t);
    return strcmp(s1, s2);

    #undef MAXLEN
}

#define MAXLINE 1000
int _getline(char *line, int lim);

int readlines(char *lineptr[], int maxlines)
{
	int nlines, len;
	char line[MAXLINE], *p;

	nlines = 0;
	while ((len = _getline(line, MAXLINE)) > 0)
		if (nlines >= maxlines || (p = malloc(len)) == NULL)
			return -1;
		else {
			strcpy(p, line);
			lineptr[nlines++] = p;
		}
	return nlines;
}

void writelines(char *lineptr[], int nlines)
{
    while (nlines-- > 0)
        printf("%s", *lineptr++);
}

int numcmp(const char *s1, const char *s2)
{
    double v1, v2;

    v1 = atof(s1);
    v2 = atof(s2);
    if (v1 < v2)
        return -1;
    if (v1 > v2)
        return 1;
    else
        return 0;
}

int _getline(char *line, int lim)
{
	int i, c;
	for (i = 0; i < lim && (c = getchar()) != EOF && c != '\n'; i++)
		line[i] = c;
	if (c == '\n')
		line[i++] = c;
	line[i] = '\0';

	return i;
}

void swap(void **v, const int i, const int j)
{
    void *temp;

    temp = v[i];
    v[i] = v[j];
    v[j] = temp;
}

void strlow(char *s)
{
    while (*s) {
        *s = tolower(*s);
        s++;
    }
    *s = '\0';
}
