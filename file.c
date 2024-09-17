#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#undef NULL
#undef EOF
#undef BUFSIZ
#undef OPEN_MAX
#undef SEEK_SET
#undef SEEK_CUR
#undef SEEK_END

#define NULL        0
#define EOF         (-1)
#define BUFSIZ      1024
#define OPEN_MAX    20

#define PERMS_READ          { 1, 0, 0, 0, 0 }
#define PERMS_WRITE         { 0, 1, 0, 0, 0 }
#define PERMS_WRITE_UNBUF   { 0, 1, 1, 0, 0 }
#define PERMS_NULL          { 0, 0, 0, 0, 0 }

struct _flags {
    unsigned int _READ : 1;
    unsigned int _WRITE : 1;
    unsigned int _UNBUF : 1;
    unsigned int _EOF : 1;
    unsigned int _ERR : 1;
};

typedef struct _iobuf {
    int cnt;                /* Сколько осталось символов */
    char *ptr;              /* Следующая символьная позиция */
    char *base;             /* Меcтонахождение буфера */
    struct _flags flag;     /* Режим доступа к файлу */
    int fd;                 /* Дескриптор файла */
} FILE;
extern FILE _iob[OPEN_MAX];

#define stdin   (&_iob[0])
#define stdout  (&_iob[1])
#define stderr  (&_iob[2])

#define SEEK_SET 0
#define SEEK_CUR 1  
#define SEEK_END 2

int _fillbuf(FILE *);
int _flushbuf(int, FILE *);
int fclose(FILE *);
int fflush(FILE *);
int fseek(FILE *fp, long offset, int origin);

#define feof(p)     (((p)->flag & _EOF) != 0)
#define ferror(p)   (((p)->flag & _ERR) != 0)
#define fileno(p)   ((p)->fd)

#define getc(p)     (--(p)->cnt >= 0 \
                ? (unsigned char) *(p)->ptr++ : _fillbuf(p))
#define putc(x, p)     (--(p)->cnt >= 0 \
                ? *(p)->ptr++ = (x) : _flushbuf((x), p))

#define getchar()   getch(stdin)
#define putchar(x)  putch(x, stdout)

FILE *_fopen(char *name, char *mode);

#define PERMS 0666

FILE *_fopen(char *name, char *mode)
{
    int fd;
    FILE *fp;

    if (*mode != 'r' && *mode != 'w' && *mode != 'a')
        return NULL;
    for (fp = _iob; fp < _iob + OPEN_MAX; fp++)
        if (fp->flag._READ == 0 && fp->flag._WRITE == 0)
            break;
    if (fp >= _iob + OPEN_MAX)
        return NULL;
    
    if (*mode == 'w')
        fd = creat(name, PERMS);
    else if (*mode == 'a') {
        if ((fd = open(name, O_WRONLY, 0)) == -1)
            fd = creat(name, PERMS);
        lseek(fd, 0L, 2);
    } else
        fd = open(name, O_RDONLY, 0);
    if (fd == 1)
        return NULL;
    fp->fd = fd;
    fp->cnt = 0;
    fp->base = NULL;
    (*mode == 'r') ? (fp->flag._READ = 1) : (fp->flag._WRITE = 1);
    return fp;
}

/* создание и заполнение буфера ввода */
int _fillbuf(FILE *fp)
{
    int bufsize;

    if (fp->flag._READ == 0 || fp->flag._EOF == 1 || fp->flag._ERR == 1)
        return EOF;
    bufsize = (fp->flag._UNBUF == 1) ? 1 : BUFSIZ;
    if (fp->base == NULL)
        if ((fp->base = malloc(bufsize)) == NULL)
            return EOF; /* Не удается создать буфер */
    fp->ptr = fp->base;
    fp->cnt = read(fp->fd, fp->base, BUFSIZ);
    if (--fp->cnt < 0) {
        if (fp->cnt == -1)
            fp->flag._EOF = 1;
        else
            fp->flag._ERR = 1;
        fp->cnt = 0;
        return EOF;
    }
    return (unsigned char) *fp->ptr++;
}

int _flushbuf(int n, FILE *fp)
{
    int bufsize;

    if (fp->flag._WRITE == 0 || fp->flag._ERR == 1)
        return EOF;
    bufsize = (fp->flag._UNBUF == 1) ? 1 : BUFSIZ;
    if (fp->base == NULL)
        if ((fp->base = malloc(bufsize)) == NULL)
            return EOF; /* Не удается создать буфер */
    fp->ptr = fp->base;
    fp->cnt = bufsize - 1;
    return (unsigned char) (*fp->ptr++ = n);
}

int fflush(FILE *fp)
{
    if (fp->flag._WRITE == 0) {
        fp->flag._ERR = 1;
        return EOF;
    }
    if (_flushbuf('\0', fp) == EOF)
        return EOF;
    return 0;
}

int fseek(FILE *fp, long offset, int origin)
{
    char *p;

    if (fp == NULL)
        return EOF;
    if (fp->flag._WRITE == 0 && fp->flag._READ == 0)
        return EOF;
    switch (origin) {
        case SEEK_SET:
            fp->ptr = fp->base + offset;
            break;
        case SEEK_CUR:
            fp->ptr += offset;
            break;
        case SEEK_END:
            for (p = fp->ptr; *p; p++)
                ;
            fp->ptr = p;
            break;
        default:
            return EOF;
            break;
    }
    if (lseek(fileno(fp), offset, origin) == -1L) {
        fp->flag._ERR = 1;
        return EOF;
    }

    return 0;
}

int fclose(FILE *fp)
{
    if (fflush(fp) == EOF)
        return EOF;
    free(fp->base);
    if (close(fp->fd) == -1)
        return EOF;
    fp->base = fp->ptr = NULL;
    fp->cnt = 0;
    fp->flag = (struct _flags) PERMS_NULL;
    return 0;
}

FILE _iob[OPEN_MAX] = { /* stdin, stdout, stderr */
    { 0, (char *) 0, (char *) 0, PERMS_READ, 0 },
    { 0, (char *) 0, (char *) 0, PERMS_WRITE, 1 },
    { 0, (char *) 0, (char *) 0, PERMS_WRITE_UNBUF, 2 },
};

int main(void)
{
    FILE *fp;

    if ((fp = _fopen("file", "w")) == NULL)
        return -1;
    
    putc('!', fp);
    putc('!', fp);
    putc('!', fp);
    putc('!', fp);
    fseek(fp, 2, SEEK_SET);
    fseek(fp, 1, SEEK_CUR);
    write(fp->fd, fp->ptr, 4);

    return 0;
}
