#include <stdio.h>
#include <string.h>
#define MAXLINE 1000

int _getline(char *line, int lim);

int main(int argc, char *argv[])
{
  char line[MAXLINE];
  long lineno = 0;
  int c, except = 0, number = 0, found = 0;

  while (--argc > 0 && (*++argv)[0] == '-')
    while ((c = *++argv[0]))
      switch (c) {
        case 'x':
          except = 1;
          break;
        case 'n':
          number = 1;
          break;
        default:
          printf("find: illegal option %c\n", c);
          argc = 0;
          found = 1;
          break;
      }
    
  if (argc != 1)
    puts("Usage: find -x -n <pattern>");
  else
    while (_getline(line, MAXLINE) > 0) {
      lineno++;
      if ((strstr(line, *argv) != NULL) != except) {
        if (number)
          printf("%ld:",lineno);
        printf("%s", line);
        found++;
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
