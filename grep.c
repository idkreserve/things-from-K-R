#include <stdio.h>
#include <string.h>

#define MAXLINE 1000

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
          fprintf(stderr, "find: illegal option %c\n", c);
          break;
      }

  if (argc != 1)
    puts("Usage: find -x -n pattern");
  else {
    while (fgets(line, MAXLINE, stdin) > 0) {
      lineno++;
      if ((strstr(line, *argv) != NULL) != except) {
        if (number)
          printf("%ld:", lineno);
        puts(line);
        found++;
      }
    }
  }
  printf("%d\n", found);
  return 0;
}
