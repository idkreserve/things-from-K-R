#include <ctype.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

void minprintf(char *fmt, ...);

int main(void)
{
    minprintf("%10s\n", "hello");
    minprintf("%-10s\n", "hello");
    minprintf("%.2s\n", "hello");
    minprintf("%.2d\n", 123);
    minprintf("%d\n", 62);
    return 0;
}

void minprintf(char *fmt, ...)
{
    union {
        int ival;
        double dval;
        char *sval;
    } u;
    va_list ap;
    int pos, repeat;
    char *p;

    pos = 0;
    va_start(ap, fmt);
    for (p = fmt; *p; p++) {
        if (*p != '%') {
            putchar(*p);
            continue;
        }
        switch (*++p) {
            case 'c':
            case 'd':
                u.ival = va_arg(ap, int);
                printf((*p == 'd') ? "%d" : "%c", u.ival);
                break;
            case 'f':
            case 'g':
                u.dval = va_arg(ap, double);
                printf("%lg", u.dval);
                break;
            case 's':
                for (u.sval = va_arg(ap, char *); *u.sval; u.sval++)
                    putchar(*u.sval);
                break;
            case '.':
                repeat = atoi(++p);
                while (isdigit(*p))
                    p++;
                if (*p != 's') {
                    if (*p == 'c' || *p == 'd') {
                        printf((*p == 'd') ? "%d" : "%c", va_arg(ap, int));
                    }
                    else
                        printf("%g", va_arg(ap, double));
                    break;
                }

                for (u.sval = va_arg(ap, char *); *u.sval && repeat > 0; u.sval++, repeat--)
                    putchar(*u.sval);
                
                break;
            case '%':
                putchar(*p);
            default:
                if (*p == '-') {
                    pos = 1;
                    p++;
                }
                repeat = atoi(p);
                while (isdigit(*p))
                    p++;

                while (pos && repeat-- > 0)
                    putchar('_');
                for ((u.sval = va_arg(ap, char *)); *u.sval; u.sval++)
                    putchar(*u.sval);
                while (repeat-- > 0)
                    putchar('_');
                pos = 0;
                break;
        }
    }
    va_end(ap);
}
