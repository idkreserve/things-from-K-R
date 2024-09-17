#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINES 5000 /* максимальное количество строк для сортировки */
#define MAXLEN 1000
#define MAXSTORE 10000
#define dprint(expr) printf(#expr " = %g\n", expr)

int readlines(char *lineptr[], char *storedptr, const int maxlines);
void writelines(char *lineptr[], int nlines);
void _qsort(char *lineptr[], int left, int right);

static char *lineptr[MAXLINES];

int main(void)
{
  int nlines; /* Количество введенных строк */
  char stored[MAXSTORE];

  if ((nlines = readlines(lineptr, stored, MAXLINES)) >= 0) {
    _qsort(lineptr, 0, nlines-1);
    putchar('\n');
    writelines(lineptr, nlines);
    return 0;
  }
  
  fprintf(stderr, "input too big to sort\n");
  return 1;
}

int _getline(char *s, int);

void _qsort(char *v[], int left, int right)
{
  int i, last;
  void _swap(char *v[], int i, int j);

  if (left >= right)
    return;
  _swap(v, left, (left+right)/2);
  last = left;
  for (i = left+1; i <= right; i++)
    if (strcmp(v[i], v[left]) < 0)
      _swap(v, ++last, i);
  _swap(v, left, last);
  _qsort(v, left, last-1);
  _qsort(v, last+1, right);
}

/* Обмен местами v[i] и v[j] */
void _swap(char *v[], int i, int j)
{
  char *temp;

  temp = v[i];
  v[i] = v[j];
  v[j] = temp;
}

/* Считывание строк из сходного потока */
int readlines(char *lineptr[], char *storedptr, const int maxlines)
{
  int len, nlines;
  char line[MAXLEN];

  nlines = 0;
  while ((len = _getline(line, MAXLEN)) > 0)
    if (nlines >= maxlines || (strlen(storedptr) + len) > MAXSTORE)
      return -1;
    else {
      line[len-1] = '\0';
      strcpy(storedptr, line);
      lineptr[nlines++] = storedptr;
      storedptr += len;
    }
  return nlines;
}

void writelines(char *lineptr[], int nlines)
{
  while (nlines-- > 0)
    puts(*lineptr++);
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
