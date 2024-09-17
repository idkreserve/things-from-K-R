#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

struct tnode {
    char *word;
    int count;
    struct tnode *left;
    struct tnode *right;
};

#define MAXWORD 100
struct tnode *addtree(struct tnode *, char *);
void treeprint(struct tnode *);
int getword(char *, int);

#define talloc() ((struct tnode *) malloc(sizeof(struct tnode)))

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

char *_strdup(char *);

struct tnode *addtree(struct tnode *p, char *w)
{
    int cond;

    if (p == NULL) {
        p = talloc();
        p->word = _strdup(w);
        p->count = 1;
        p->left = p->right = NULL;
    } else if ((cond = strcmp(w, p->word)) == 0)
        p->count++;
    else if (cond < 0)
        p->left = addtree(p->left, w);
    else
        p->right = addtree(p->right, w);
    return p;
}

void treeprint(struct tnode *p)
{
    if (p != NULL) {
        treeprint(p->left);
        printf("%4d %s\n", p->count, p->word);
        treeprint(p->right);
    }
}

char *_strdup(char *s)
{
    char *p;

    p = (char *) malloc(strlen(s)+1); /* +1 для '\0' */
    if (p != NULL)
        strcpy(p, s);
    return p;
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
