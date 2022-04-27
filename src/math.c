/* math.c - math funcs */

#include "rogue.h"

int abs_int(int i)
{
        return i < 0 ? -i : i;
}

double power(int x, int y)
{
        int i, count, sign;
        if (y < 0) {
                sign = 1;
                y *= -1;
        } else {
                sign = 0;
        }
        for (i = 1, count = 0; count < y; count++)
                i *= x;
        return sign ? 1.0 / i : i;
}
