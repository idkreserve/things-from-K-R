#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define MAXWORD 100
#define NKEYS   (sizeof keytab / sizeof keytab[0])

struct key {
  char *word;
  int count;
} keytab[] = {
    {"while", 0},    {"auto", 0},     {"break", 0},    {"case", 0},
    {"char", 0},     {"const", 0},    {"continue", 0}, {"default", 0},
    {"do", 0},       {"double", 0},   {"else", 0},     {"enum", 0},
    {"extern", 0},   {"float", 0},    {"for", 0},      {"goto", 0},
    {"if", 0},       {"int", 0},      {"long", 0},     {"register", 0},
    {"return", 0},   {"short", 0},    {"signed", 0},   {"size_t", 0},
    {"sizeof", 0},   {"static", 0},   {"struct", 0},   {"switch", 0},
    {"typedef", 0},  {"union", 0},    {"unsigned", 0}, {"void", 0},
    {"volatile", 0}, {"restrict", 0},
};

int getword(char *, int);
struct key *binsearch(char *, struct key *, int);

int main(void)
{
    char word[MAXWORD];
    struct key *p;

    while (getword(word, MAXWORD) != EOF)
        if (isalpha(word[0]))
            if ((p = binsearch(word, keytab, NKEYS)) != NULL)
                p->count++;
    for (p = keytab; p < keytab + NKEYS; p++) /* adr: 1000, n: 10 */
        if (p->count > 0)
            printf("%4d %s\n", p->count, p->word);
    return 0;
}

int getword(char *word, int lim)
{
    int c, type;
    char *w = word;

    while (isspace(c = getchar()))
        ;
    if (c != EOF)
        *w++ = c;
    if (isalpha(c) || c == '#' || c == '_') {
        for ( ; --lim > 0; w++)
            if (!isalnum(*w = getchar()) && *w != '_') {
                ungetc(*w, stdin);
                break;
            }
    } else if ((type = c) == '\'' || c == '\"') {
        while ((c = getchar()) != type && c != EOF)
            if (c == '\\')
                getchar();
    } else if (c == '/') {
        if ((type = c = getchar()) != '/' && c != '*')
            ungetc(c, stdin);
        else {
            if (type == '/')
                while ((c = getchar()) != '\n' && c != EOF)
                    ;
            else
                while ((c = getchar()) != EOF)
                    if (c == '*' && (c = getchar()) == '/')
                        break;
        }
    } else if (!isalpha(c)) {
        *w = '\0';
        return c;
    }
    *w = '\0';
    return word[0];
}

struct key *binsearch(char *word, struct key tab[], int n)
{
    int cond;
    struct key *low = &tab[0];
    struct key *high = &tab[n];
    struct key *mid;

    while (low <= high) {
        mid = low + (high-low) / 2;
        if ((cond = strcmp(word, mid->word)) < 0)
            high = mid;
        else if (cond > 0)
            low = mid + 1;
        else
            return mid;
    }
    return NULL;
}
