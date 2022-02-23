/* level.c - level control*/

#include "rogue.h"

int is_ok(int x, int y)
{
        return x >= 0 && x < gamew_col && y >= 0 && y < gamew_row;
}

int is_empty(struct level *l, int x, int y)
{
        return is_ok(x, y) && !is_room(l->r, x, y) &&
               !is_path(l->p, x, y) && !is_door(l->d, x, y);
}

void init_level(struct level *l)
{
        l->depth = init_room(&l->r);
        init_path(l);
}

void end_level(struct level *l)
{
        free_room(l->r);
        free_path(l->p);
        free_door(l->d);
}
