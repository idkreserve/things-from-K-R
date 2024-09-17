#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define MAXWORD     100
#define EQU_FIRST   6
#define NAME        '0'
#define TYPE        '1'
#define NTYPES      (sizeof types / sizeof types[0])

static char *types[] = {
    "char", "double", "float", "int", "long", "short",
};
static char *patternptr[MAXWORD+1];

struct tnode {
  char *name;
  struct tnode *left;
  struct tnode *right;
};
#define talloc()      ((struct tnode *) malloc(sizeof(struct tnode)))

int tokentype;
char token[MAXWORD];

static int gettoken(void);
static int binsearch(char *, char *[], int);
static struct tnode *addtree(struct tnode *);
static void treeprint(struct tnode *, char *);
static char *strstrip(char *, int);

int main(int argc, char *argv[])
{
    int i, n = (argc == 2) ? atoi(argv[1]) : EQU_FIRST;
    struct tnode *root;

    root = NULL;
    i = 0;
    while (gettoken() != EOF) {
        if (tokentype == NAME) {
            patternptr[i++] = strstrip(token, n);
            root = addtree(root);
        }
        patternptr[i] = NULL;
    }
    for (i = 0; patternptr[i] != NULL; i++) {
        treeprint(root, patternptr[i]);
        if (patternptr[i+1] != NULL)
            putchar('\n');
    }
    return 0;
}

static struct tnode *addtree(struct tnode *p)
{
    int cond;

    if (p == NULL) {
        p = talloc();
        p->name = strdup(token);
        p->left = p->right = NULL;
    } else if ((cond = strcmp(token, p->name)) < 0)
        p->left = addtree(p->left);
    else if (cond > 0)
        p->right = addtree(p->right);
    return p;
}

static void treeprint(struct tnode *p, char *pattern)
{
    if (p != NULL) {
        treeprint(p->left, pattern);
        if (strstr(p->name, pattern) != NULL)
            puts(p->name);
        treeprint(p->right, pattern);
    }
}

static int gettoken(void)
{
    int c, i, type;
    char *pt = token;

    while (isspace(c = getchar()))
        ;
    if (isalpha(c)) {
        for (i = 0, *pt++ = c; i < MAXWORD-1 && isalnum(c = getchar()); i++, *pt++ = c)
            ;
        ungetc(c, stdin);
        *pt = '\0';
        return tokentype = (binsearch(token, types, NTYPES) < 0) ? NAME : TYPE;
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
    }
    *pt = '\0';
    return tokentype = c;
}

static int binsearch(char *word, char *tab[], int n)
{
    int low, mid, high;
    int cond;

    low = 0;
    high = n-1;
    while (low <= high) {
        mid = (low+high) / 2;
        if ((cond = strcmp(word, tab[mid])) < 0)
            high = mid - 1;
        else if (cond > 0)
            low = mid + 1;
        else
            return mid;
    }
    return -1;
}

static char *strstrip(char *s, int n)
{
    char *line = malloc(n+1), *p = line;
    while (n-- > 0)
        *p++ = *s++;
    *p = '\0';
    return line;
}
