#include <stdio.h>

#define MAXMON  12
#define MAXDAY  31
#define MAXYDAY(is_leap) ((is_leap) ? 366 : 365)

static char not_leap[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
static char leap[] = { 0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

static char *daytab[] = { not_leap, leap };

int day_of_year(int year, int month, int day)
{
    if (year <= 0 || month <= 0 || day <= 0 || month > MAXMON || day > MAXDAY)
        return -1;

    int i, leap;
    
    leap = (year%4 == 0 && year%100 != 0 ) || year%400 == 0;
    for (i = 1; i < month; i++)
        // day += daytab[leap][i];
        day += (*(daytab + leap))[i];
    return day;
}

void month_day(int year, int yearday, int *pmonth, int *pday)
{
    int i, leap;
    leap = (year%4 == 0 && year%100 != 0 ) || year%400 == 0;
    if (year <= 0 || yearday <= 0 || yearday > MAXYDAY(leap)) {
        *pmonth = *pday = -1;
        return;
    }
    
    for (i = 1; yearday > daytab[leap][i]; i++)
        // yearday -= daytab[leap][i];
        yearday -= (*(daytab + leap))[i];
    *pmonth = i;
    *pday = yearday;
}

int main(void)
{
    // int pmonth, pday;

    // month_day(2024, 21, &pmonth, &pday);
    // printf(" mon %d, day %d\n", pmonth, pday);
    printf("%d\n", day_of_year(2024, 2, 31));
    return 0;
}
