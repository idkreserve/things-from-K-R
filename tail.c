#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define N 10
#define MAXLEN 5000

#define max(a, b) ((a > b) ? a : b)

char *lineptr[MAXLEN];

int readlines(char *lineptr[], int maxlines);
void writelines(char *lineptr[], int maxlines, int n);

int main(int argc, char *argv[])
{
	int n = N, nlines;
	if (argc == 2 && (*++argv)[0] == '-')
		n = (isdigit(*++argv[0])) ? atoi(argv[0]) : N;

	if ((nlines = readlines(lineptr, MAXLEN)) >= 0)
		writelines(lineptr, nlines, n);
}

#define MAXLINE 1000
int _getline(char *line, int lim);

void writelines(char *lineptr[], int maxlines, int n)
{
	int current;
	for (current = max(maxlines-n, 0); current < maxlines; current++)
		printf("%s", lineptr[current]);
}

int readlines(char *lineptr[], int maxlines)
{
	int nlines, len;
	char line[MAXLINE], *p;

	nlines = 0;
	while ((len = _getline(line, MAXLINE)) > 0)
		if (nlines >= maxlines || (p = malloc(len)) == NULL)
			return -1;
		else {
			// line[len-1] = '\0';
			strcpy(p, line);
			lineptr[nlines++] = p;
		}
	return nlines;
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
