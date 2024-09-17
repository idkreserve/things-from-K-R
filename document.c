#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define dmemset(mem, n)                                 \
for (int i = 0; i < n; i++)                             \
    mem[i] = 0;                                         \

#define dmemprint(mem, n)                               \
printf("[ ");                                           \
for (int i = 0; i < n; i++)                             \
    if (mem[i] > 0)                                     \
        printf("%d ", mem[i]);                          \
puts("]");

#define MAXLINES    1000
#define MAXWORD     100

static int line = 1;

struct tnode {
    char *word;
    int lines[MAXLINES];
    int *lineptr;
    struct tnode *left;
    struct tnode *right;
};
#define talloc() ((struct tnode *) malloc(sizeof(struct tnode)))

int getword(char *, int);
struct tnode *addtree(struct tnode *, char *);
void treeprint(struct tnode *);
int *arrhas(int [], int, unsigned int);

int main(void)
{
    struct tnode *root;
    char word[MAXWORD];

    root = NULL;
    while (getword(word, MAXWORD) != EOF)
        if (isalpha(word[0]))
            root = addtree(root, word);
    treeprint(root);
    return 0;
}

struct tnode *addtree(struct tnode *p, char *w)
{
    int cond;

    if (p == NULL) {
        p = talloc();
        p->word = strdup(w);
        p->lineptr = p->lines;
        p->left = p->right = NULL;

        dmemset(p->lines, MAXLINES);
        *p->lineptr++ = line;
    } else if ((cond = strcmp(w, p->word)) == 0 && arrhas(p->lines, line, MAXLINES) == NULL)
        *p->lineptr++ = line;
    else if (cond < 0)
        p->left = addtree(p->left, w);
    else if (cond > 0)
        p->right = addtree(p->right, w);
    return p;
}

void treeprint(struct tnode *p)
{
    if (p != NULL) {
        treeprint(p->left);
        printf("%10s - ", p->word);
        dmemprint(p->lines, MAXLINES);
        treeprint(p->right);
    }
}

int getword(char *word, int lim)
{
    int c, type;
    char *w = word;

    while ((c = getchar()) == ' ' || c == '\t')
        ;
    if (c != EOF)
        *w++ = c;
    if (isalpha(c) || c == '#' || c == '_') {
        for ( ; --lim > 0; w++)
            if (!isalnum(*w = getchar()) && *w != '_' && *w != '\'' && *w != '-') {
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
        if (c == '\n')
            line++;
        *w = '\0';
        return c;
    }
    if (c == '\n')
        line++;
    *w = '\0';
    return word[0];
}

int *arrhas(int arr[], int n, unsigned int size)
{
    int i;

    for (i = 0; i < size; i++)
        if (arr[i] == n)
            return arr + i;
    return NULL;
}
