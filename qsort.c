#include <stdio.h>
#include <stdlib.h>

// printf("%d %d\n", v[0], v[(left+right)/2]);

// for (i = 0; i < 8; i++)
//   printf("%d ", v[i]);
// putchar('\n');

int buf[] = { 5, 2, 8, 6, 3, 9 };
#define bufsize sizeof(buf) / sizeof(int)

void _qsort(int v[], int left, int right);

int main(void)
{
  int i;
  _qsort(buf, 0, bufsize);
  return 0;
}

void _qsort(int v[], int left, int right)
{
  int last, i;
  void _swap(int v[], int i, int j);

  if (left >= right) {
    return;
  }
  _swap(v, left, (left+right)/2);
  last = left;
  for (i = left+1; i <= right; i++)
    if (v[i] < v[left])
      _swap(v, ++last, i);
  _swap(v, left, last);

  for (i = 0; i < bufsize; i++)
    printf("%d\n", v[i]);
}

void _swap(int v[], int i, int j)
{
  int temp;

  temp = v[i];
  v[i] = v[j];
  v[j] = temp;
}
