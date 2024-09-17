#include <stdio.h>
#include <string.h>
#define MAXLINE 1000

int _getline(char *line, int lim);

int main(int argc, char *argv[])
{
  char line[MAXLINE];
  int found = 0;

  if (argc != 2)
    puts("Usage: find <pattern>");
  else {
    while (_getline(line, MAXLINE) > 0) {
      if (strstr(line, argv[1]) != NULL) {
        puts(line);
        found++;
      }
    }
  }
  printf("%d\n", found);
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
