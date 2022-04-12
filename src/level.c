/* level.c - level control*/

#include <stdlib.h>

#include "rogue.h"

enum {
        start_point_symb = '<',
        end_point_symb   = '>'
};

int is_linked_coord(struct linked_coord *lc, int x, int y)
{
        for ( ; lc; lc = lc->next)
                if (lc->pos.x == x && lc->pos.y == y)
                        return 1;
        return 0;
}

void add_linked_coord(struct linked_coord **lc, int x, int y)
{
        struct linked_coord *t;
        if (is_linked_coord(*lc, x, y))
                return;
        t = malloc(sizeof(*t));
        t->pos.x = x;
        t->pos.y = y;
        t->next = *lc;
        *lc = t;
}

void free_linked_coord(struct linked_coord *lc)
{
        struct linked_coord *t;
        while (lc) {
                t = lc;
                lc = lc->next;
                free(t);
        }
}

int is_ok(int x, int y)
{
        return x >= 0 && x < gamew_col && y >= 0 && y < gamew_row;
}

int is_empty(const struct level *l, int x, int y)
{
        return is_ok(x, y) && !is_room(l->r, x, y) &&
               !is_path(l->p, x, y) && !is_door(l->d, x, y);
}

int is_stop(const struct level *l, int x, int y)
{
        const struct door *d;
        const struct loot_list *ll;
        if ((l->start.pos.x == x && l->start.pos.y == y) ||
            (l->end.pos.x   == x && l->end.pos.y   == y))
                return 1;
        for (d = l->d; d; d = d->next)
                if (d->pos.x == x && d->pos.y == y)
                        return 1;
        for (ll = l->l; ll; ll = ll->next)
                if (ll->pos.x == x && ll->pos.y == y)
                        return 1;
        return 0;
}

int can_move(const struct level *l, const struct creature *h, int x, int y)
{
        return !is_hunter(h, x, y) && !is_beast(l->b, x, y) &&
               ((is_room(l->r, x, y) && (!is_wall(l->r, x, y) ||
                is_door(l->d, x, y))) || is_path(l->p, x, y));
}

int get_char_side(int c)
{
        switch (c) {
        case 'h': case 'H': return side_west;
        case 'j': case 'J': return side_south;
        case 'k': case 'K': return side_north;
        case 'l': case 'L': return side_east;
        case 'y': case 'Y': return side_northwest;
        case 'u': case 'U': return side_northeast;
        case 'b': case 'B': return side_southwest;
        case 'n': case 'N': return side_southeast;
        default:            return -1;
        }
}

void get_side_diff(int side, int *dx, int *dy)
{
        switch (side) {
        case side_northwest:
                *dx = -1;
                *dy = -1;
                break;
        case side_north:
                *dx = 0;
                *dy = -1;
                break;
        case side_northeast:
                *dx = 1;
                *dy = -1;
                break;
        case side_east:
                *dx = 1;
                *dy = 0;
                break;
        case side_southeast:
                *dx = 1;
                *dy = 1;
                break;
        case side_south:
                *dx = 0;
                *dy = 1;
                break;
        case side_southwest:
                *dx = -1;
                *dy = 1;
                break;
        case side_west:
                *dx = -1;
                *dy = 0;
                break;
        default:
                *dx = 0;
                *dy = 0;
        }
}

int can_side(const struct level *l, int side, int x, int y)
{
        int dx, dy;
        get_side_diff(side, &dx, &dy);
        x += dx;
        y += dy;
        return !is_beast(l->b, x, y) && ((is_room(l->r, x, y) &&
               (!is_wall(l->r, x, y) || is_door(l->d, x, y))) ||
                is_path(l->p, x, y));
}

/* Return side random side if can`t go throught first one */
int try_side(const struct level *l, int side, int x, int y)
{
        if (can_side(l, side, x, y)) {
                return side;
        } else {
                if (rand() % 2) {
                        if (can_side(l, side-1, x, y))
                                return side - 1;
                        else if (can_side(l, side+1, x, y))
                                return side + 1;
                } else {
                        if (can_side(l, side+1, x, y))
                                return side + 1;
                        else if (can_side(l, side-1, x, y))
                                return side - 1;
                }
        }
        return rand() % 8;
}

static void init_points(struct level *l)
{
        struct room *r;
        r = get_room_by_idx(l->r, 'A', 1);
        l->start.pos.x = r->tl.x + 1 + rand() % (room_len(r, 'h') - 2);
        l->start.pos.y = r->tl.y + 1 + rand() % (room_len(r, 'v') - 2);
        l->start.symb = start_point_symb;
        l->start.flags = 0;
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
        l->end.pos.x = r->tl.x + 1 + rand() % (room_len(r, 'h') - 2);
        l->end.pos.y = r->tl.y + 1 + rand() % (room_len(r, 'v') - 2);
        l->end.symb = end_point_symb;
        l->end.flags = 0;
}

void init_level(struct level *l, struct creature *h)
{
        l->depth = init_room(&l->r);
        init_path(l);
        init_points(l);
        init_loot(l, h);
        init_hunter(h, l);
        init_beast(l, h);
}

void end_level(struct level *l)
{
        free_room(l->r);
        free_path(l->p);
        free_door(l->d);
        free_beast(l->b);
        free_loot(l->l);
        wclear(msgw);
        wclear(gamew);
        wclear(infow);
        wrefresh(msgw);
        wrefresh(gamew);
        wrefresh(infow);
}
