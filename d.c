#include <stdio.h>
#include <unistd.h>

typedef long Align;

void *memalloc(unsigned);
void memfree(void *);

typedef union header {
    struct {
        union header *ptr;
        unsigned size;
    } s;
    Align __pad;
} Header;

static Header base;             /* пустой список для начала */
static Header *freep = NULL;    /* начало списка */

int main(void)
{
    char *buf;

    if ((buf = memalloc(200)) == NULL)
        fputs("sucks!", stderr);
    return 0;
}

/* функция распределения памяти */
void *memalloc(unsigned nbytes)
{
    Header *p, *prevp;
    Header *morecore(unsigned);
    unsigned nunits;

    nunits = (nbytes+sizeof(Header)-1) / sizeof(Header) + 1;
    if ((prevp = freep) == NULL) {
        base.s.ptr = freep = prevp = &base;
        base.s.size = 0;
    }
    for (p = prevp->s.ptr; ; prevp = p, p = p->s.ptr) {
        if (p->s.size >= nunits) {      /* достаточный размер */
            if (p->s.size == nunits)    /* в точности */
                prevp->s.ptr = p->s.ptr;
            else {  /* отрезаем "хвост" */
                p->s.size -= nunits;
                p += p->s.size;
                p->s.size = nunits;
            }
            freep = prevp;
            return (void *) (p+1);
        }
        if (p == freep) /* ссылается на сам список */
            if ((p = morecore(nunits)) == NULL)
                return NULL;    /* не остлось памяти */
    }
}

#define NALLOC 1024 /* минимально запрашиваемое количество блоков */

/* запрос дополнительной памяти у системы */
Header *morecore(unsigned nu)
{
    char *cp;
    Header *up;

    if (nu < NALLOC)
        nu = NALLOC;
    cp = sbrk(nu * sizeof(Header));
    if (cp == (char *) -1)  /* места в памяти нет */
        return NULL;
    up = (Header *) cp;
    up->s.size = nu;
    memfree((void *) (up+1));
    return freep;
}

/* помещение блока ap в список свободных блоков */
void memfree(void *ap)
{

}
