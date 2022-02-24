/* level.c - level control*/

#include <stdlib.h>

#include "rogue.h"

int is_ok(int x, int y)
{
        return x >= 0 && x < gamew_col && y >= 0 && y < gamew_row;
}

int is_empty(const struct level *l, int x, int y)
{
        return is_ok(x, y) && !is_room(l->r, x, y) &&
               !is_path(l->p, x, y) && !is_door(l->d, x, y);
}

static void init_points(struct level *l)
{
        struct room *r;
        r = get_room_by_idx(l->r, 'A', 1);
        l->start.x = r->tl.x + 1 + rand() % (room_len(r, 'h') - 2);
        l->start.y = r->tl.y + 1 + rand() % (room_len(r, 'v') - 2);
        switch (l->depth) {
        case 1:
                r = get_room_by_idx(l->r, 'A', 2);
                break;
        case 2:
                r = get_room_by_idx(l->r, 'B', 2);
                break;
        case 3:
                r = get_room_by_idx(l->r, 'B', 4);
                break;
        case 4:
                r = get_room_by_idx(l->r, 'D', 4);
                break;
        }
        l->end.x = r->tl.x + 1 + rand() % (room_len(r, 'h') - 2);
        l->end.y = r->tl.y + 1 + rand() % (room_len(r, 'v') - 2);
}

void init_level(struct level *l, struct hunter *h)
{
        l->depth = init_room(&l->r);
        init_path(l);
        init_points(l);
        init_hunter(h, l);
}

void end_level(struct level *l)
{
        free_room(l->r);
        free_path(l->p);
        free_door(l->d);
}

void show_points(struct coord start, struct coord end)
{
        mvwaddch(gamew, start.y, start.x, '{');
        mvwaddch(gamew, end.y, end.x, '}');
}
