#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct macro {
    char *key;
    char *value;
};

struct nlist {  /* запись таблицы */
    struct nlist *next; /* следующая запись в цепочке */
    char *name;         /* имя в #define */
    char *defn;         /* подставляемый текст */
};

#define HASHSIZE 101
static struct nlist *hashtab[HASHSIZE]; /* таблица указателей */

/* формирование хэш-кода для строки s */
unsigned hash(char *s)
{
    unsigned hashval;

    for (hashval = 0; *s != '\0'; s++)
        hashval = *s + 31 * hashval;
    return hashval % HASHSIZE;
}

/* lookup: поиск элемента s в таблице hashtab */
struct nlist *lookup(char *s)
{
    struct nlist *np;

    for (np = hashtab[hash(s)]; np != NULL; np = np->next)
        if (strcmp(s, np->name) == 0)
            return np; /* Элемент найден */
    return NULL; /* Элемент не найден */
}

struct nlist *lookup(char *);

/* Помещает запись "Имя+Определение"
    (name, defn) в табоицу hashtab */
struct nlist *install(char *name, char *defn)
{
    struct nlist *np;
    unsigned hashval;

    if ((np = lookup(name)) == NULL) {  /* имя не найдено */
        if ((np = malloc(sizeof *np) ) == NULL || (np->name = strdup(name)) == NULL)
            return NULL;
        hashval = hash(name);
        np->next = hashtab[hashval];
        hashtab[hashval] = np;
    } else
        free(np->defn);
    if ((np->defn = strdup(defn)) == NULL)
        return NULL;
    return np;
}

struct nlist *install(char *s, char *t);
struct macro getmacro(char *s);
void *undef(char *);

#define MAXLINE 1000

int main(void)
{
    char line[MAXLINE];
    struct macro m;

    while (fgets(line, MAXLINE, stdin))
        if (strcmp((m = getmacro(line)).key, "\0") != 0 && strcmp(m.value, "\0") != 0) {
            install(strdup(m.key), strdup(m.value));
            free(m.key);
            free(m.value);
        }

    puts(hashtab[hash("MAXWORD")]->defn);

    return 0;
}

#define MAXWORD 1000

struct macro getmacro(char *s)
{
    int i;
    char macn[MAXWORD], key[MAXWORD], value[MAXWORD];
    struct macro m = { "", "" };

    while (*s != '\0' && *s != '#')
        s++;

    for (i = 0; i < MAXWORD-1 && *s != '\0' && *s != ' '; s++)
        macn[i++] = *s;
    macn[i] = '\0';

    if (strcmp(macn, "#define") != 0)
        return m;

    while (isspace(*s))
        s++;
    for (i = 0; i < MAXWORD-1 && *s != '\0' && *s != ' ' && *s != '\n'; s++)
        key[i++] = *s;
    key[i] = '\0';

    while (isspace(*s))
        s++;
    for (i = 0; i < MAXWORD-1 && *s != '\0' && *s != ' ' && *s != '\n'; s++)
        value[i++] = *s;
    value[i] = '\0';

    m.key = strdup(key);
    m.value = strdup(value);

    return m;
}

void *undef(char *name)
{
    struct nlist *np, *prev;
    unsigned hashval = hash(name);

    for (prev = NULL, np = hashtab[hashval];
        prev != NULL;
        prev = np, np = np->next)
            if (strcmp(np->name, name) == 0) {
                free(np->name);
                free(np->defn);

                if (prev == NULL)
                    hashtab[hashval] = np->next;
                else
                    prev->next = np->next;
                free(np);
                break;
            }
    return NULL;
}
