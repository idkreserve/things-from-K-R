#include <stdio.h>
#include <ctype.h>

#define MAXVAL 1000
#define NUMBER '0'

static double stack[MAXVAL];
static unsigned sp = 0;

static void push(double);
static double pop(void);
static int getop(double *);

int main(void)
{
    double f, op2;
    int type;
    
    while ((type = getop(&f)) != EOF)
        switch (type) {
            case NUMBER:
                push(f);
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
                if (op2 > 0)
                    push(pop() / op2);
                else
                    fputs("zero divisor", stderr);
                break;
            case '\n':
                printf("\t%.8lg\n", pop());
                break;
            default:
                puts("unknown command");
                break;
        }
    return 0;
}

static int getop(double *f)
{
    int c;

    while ((c = getchar()) == ' ' || c == '\t')
        ;
    if (!isdigit(c) && c != '.') {
        *f = 0.0;
        return c;
    }

    ungetc(c, stdin);
    scanf("%lg", f);
    return NUMBER;
}

static void push(double f)
{
    if (sp < MAXVAL)
        stack[sp++] = f;
    else
        fputs("error: stack full, can't push", stderr);
}

static double pop(void)
{
    if (sp > 0)
        return stack[--sp];
    fputs("error: stack empty", stderr);
    return 0.0;
}
