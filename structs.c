#include <stdio.h>

struct point {
    int x;
    int y;
};

struct rect {
    struct point pt1;
    struct point pt2;
};

struct {
    int len;
    char *str;
} *p;

struct point pt = { 320, 200 };
struct rect screen = { { 1, 2 }, { 7, 8 } };

struct key {
    char *word;
    int count;
} keytab[] = {
    "auto", 0,
    "break", 0,
    "case", 0,
    "char", 0,
    "const", 0,
    "continue", 0,
    "default", 0,
    "unsigned", 0,
    "void", 0,
    "volatile", 0,
    "while", 0,
};

int main(void)
{
    #define XMAX 500
    #define YMAX 1000

    struct rect screen;
    struct point middle;
    struct point makepoint(int, int);

    screen.pt1 = makepoint(0, 0);
    screen.pt2 = makepoint(XMAX, YMAX);
    middle = makepoint((screen.pt1.x + screen.pt2.x)/2,
                       (screen.pt1.y + screen.pt2.y)/2);

    struct point origin = {6, 7}, *pp = &origin;
    printf("origin is (%d %d)\n", pp->x, pp->y);

    ++(p->len);
    #undef XMAX
    #undef YMAX
    return 0;
}

struct point makepoint(int x, int y)
{
    struct point temp;

    temp.x = x;
    temp.y = y;
    return temp;
}

struct point addpoint(struct point p1, struct point p2)
{
    p1.x += p2.x;
    p1.y += p2.y;
    return p1;
}

int ptinrect(struct point p, struct rect r)
{
    return p.x >= r.pt1.x && p.x < r.pt2.x
        && p.y >= r.pt1.y && p.y < r.pt2.y;
}

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))
struct rect canonrect(struct rect r)
{
    struct rect temp;

    temp.pt1.x = min(r.pt1.x, r.pt2.x);
    temp.pt1.y = min(r.pt1.y, r.pt2.y);
    temp.pt2.x = min(r.pt1.x, r.pt2.x);
    temp.pt2.y = min(r.pt1.y, r.pt2.y);
    return temp;
}
