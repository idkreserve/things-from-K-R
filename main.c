#include <stdio.h>

char *foo(int n)
{
  static char *name[] = {
    "odin",
    "dva",
    "tri",
  };

  return (n > 0 && n < 4) ? name[n] : "goodbye, loh";
}

int main(void)
{
  char *line = "hello world";
  char *lineptr[2] = { "hello world", "chlen" };
  printf("%s\n", line);
  printf("%s\n", *lineptr);

  int buf[2][5] = {
    { 1, 2, 3, 4, 5 },
    { 6, 7, 8, 9, 0 },
  };
  int (*ptr)[5] = buf;

  printf("%d\n", (*++ptr)[3]);

  char *kek = foo(2);
  printf("%c\n", *kek);

  int z = 1, x = 2, c = 3;
  int a[2][4] = { { 1, 2 }, { 3, 4 } };
  int *b[2] = { a[0], a[1] };
  printf("%d, ", **b);

  return 0;
}
