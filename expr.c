#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#define MAXOP  100
#define MAXVAL 100
#define NUMBER '0'
// printf("Signal %c for operator %s\n", getop(line, *++argv), line);
int getop(char *s, char *arg);
void push(double f);
double pop(void);

static double stack[MAXVAL];
static int sp = 0;
#define STACKLEN sizeof(stack) / sizeof(double)

int main(int argc, char *argv[])
{
  if (argc == 1) {
    fprintf(stderr, "Usage ./expr <op> <op> <num>\n");
    return 1;
  }

  double op2;
  char type, line[MAXOP];

  while (--argc > 0) {
    switch ((type = getop(line, *++argv))) {
      case NUMBER:
        push(atof(line));
        break;
      case '+':
        push(pop() + pop());
        break;
      case '-':
        op2 = pop();
        push(pop() - op2);
        break;
      case '*':
        push(pop() * pop());
        break;
      case '/':
        op2 = pop();
        if (op2 == 0)
          puts("Error: zero divisor");
        else
         push(pop() / op2);
        break;
      default:
        printf("Error: unknown command %s\n", line);
        break;
    }
  }
  printf("%g\n", stack[0]);

  return 0;
}

// void printstack(void)
// {
  // int i;
  
  // for (i = 0; i < STACKLEN; i++)
  //   printf("%g\n", stack[i]);
// }

void push(double f)
{
  if (sp < MAXVAL)
    stack[sp++] = f;
  else
    puts("Error: stack full, can't push");
}

double pop(void)
{
  if (sp > 0)
    return stack[--sp];

  puts("Error: stack empty");
  return 0.0;
}

int getop(char *s, char *arg)
{
  if (!isdigit(*s = *arg) && *s != '.')
    return *s;

  if (isdigit(*arg))
    while (isdigit(*s++ = *arg++))
    ;
  if (*(s-1) == '.')
    while (isdigit(*s++ = *arg++))
      ;
  *(s-1) = '\0';
  return NUMBER;
}

