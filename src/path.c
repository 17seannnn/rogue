/* path.c - path generating */

#include <stdlib.h>
#include <curses.h>

#include "rogue.h"

enum {
        pave_steps_range = 1
};

static void add_path(struct path **p, int x, int y)
{
        struct path *t;
        t = malloc(sizeof(*t));
        t->pos.x = x;
        t->pos.y = y;
        t->next  = *p;
        t->flags = 0;
        *p = t;
}

static void add_door(struct door **d, struct room *owner, int x, int y)
{
        struct door *t;
        t        = malloc(sizeof(*t));
        t->pos.x = x;
        t->pos.y = y;
        t->owner = owner;
        t->next  = *d;
        t->flags = 0;
        *d = t;
}

struct path *get_path_by_coord(const struct path *p, int x, int y)
{
        for ( ; p; p = p->next)
                if (p->pos.x == x && p->pos.y == y)
                        return (struct path *)p;
        return NULL;
}

int is_path(const struct path *p, int x, int y)
{
        return get_path_by_coord(p, x, y) ? 1 : 0;
}

int is_door(const struct door *d, int x, int y)
{
        for ( ; d; d = d->next)
                if (d->pos.x == x && d->pos.y == y)
                        return 1;
        return 0;
}

static int pave_hor(struct level *l, int b_x, int b_y, int e_x)
{
        int steps;
        steps = rand() % pave_steps_range + 1;
        if (steps > abs_int(e_x - b_x) + 1)
                steps = abs_int(e_x - b_x) + 1;
        for ( ; steps > 0; steps--) {
                if (is_empty(l, b_x, b_y)) {
                        add_path(&l->p, b_x, b_y);
                } else {
                        if (is_wall(l->r, b_x, b_y) &&
                            !is_door(l->d, b_x, b_y))
                                add_door(&l->d,
                                         get_room_by_coord(l->r, b_x, b_y),
                                         b_x, b_y);
                }
                if (e_x - b_x > 0)
                        b_x++;
                else
                if (e_x - b_x < 0)
                        b_x--;
                else
                        break;
        }
        return b_x;
}

static int pave_ver(struct level *l, int b_x, int b_y, int e_y)
{
        int steps;
        steps = rand() % pave_steps_range + 1;
        if (steps > abs_int(e_y - b_y) + 1)
                steps = abs_int(e_y - b_y) + 1;
        for ( ; steps > 0; steps--) {
                if (is_empty(l, b_x, b_y)) {
                        add_path(&l->p, b_x, b_y);
                } else {
                        if (is_wall(l->r, b_x, b_y))
                                add_door(&l->d,
                                           get_room_by_coord(l->r, b_x, b_y),
                                           b_x, b_y);
                }
                if (e_y - b_y > 0)
                        b_y++;
                else
                if (e_y - b_y < 0)
                        b_y--;
                else
                        break;
        }
        return b_y;
}

static void pave_path(struct level *l, struct room *r1, struct room *r2)
{
        int order;
        int b_x, b_y, e_x, e_y;
        /* Coordinates are inside room, not in walls */
        b_x = r1->tl.x + 1 + rand() % (room_len(r1, 'h') - 2);
        b_y = r1->tl.y + 1 + rand() % (room_len(r1, 'v') - 2);
        e_x = r2->tl.x + 1 + rand() % (room_len(r2, 'h') - 2);
        e_y = r2->tl.y + 1 + rand() % (room_len(r2, 'v') - 2);
        order = rand() % 2;
        for (;;) {
                if (order) {
                        b_x = pave_hor(l, b_x, b_y, e_x);
                        b_y = pave_ver(l, b_x, b_y, e_y);
                } else {
                        b_y = pave_ver(l, b_x, b_y, e_y);
                        b_x = pave_hor(l, b_x, b_y, e_x);
                }
                if (b_x == e_x && b_y == e_y)
                        break;
        }
}

static void fix_door(struct level *l)
{
        struct door **d = &l->d, *t;
        struct path *p = l->p;
        while (*d) {
                if (!is_path(p, (*d)->pos.x-1, (*d)->pos.y) &&
                    !is_path(p, (*d)->pos.x+1, (*d)->pos.y) &&
                    !is_path(p, (*d)->pos.x, (*d)->pos.y-1) &&
                    !is_path(p, (*d)->pos.x, (*d)->pos.y+1)) {
                        t = *d;
                        *d = (*d)->next;
                        free(t);
                } else {
                        d = &(*d)->next;
                }
        }
}
void init_path(struct level *l)
{
        int c, n, next_c, next_n;
        struct room *r1, *r2;
        l->p = NULL;
        l->d = NULL;
        c = 'A';
        n = 1;
        next_c = c;
        next_n = n + 1;
        while ((r1 = get_room_by_idx(l->r, c, n)) &&
               (r2 = get_room_by_idx(l->r, next_c, next_n))) {
                pave_path(l, r1, r2);
                c = next_c;
                n = next_n;
                if (next_n >= (l->depth <= 2 ? 2 : 4)) {
                        next_c++;
                        next_n = 1;
                } else {
                        next_n++;
                }
        }
        fix_door(l);
}

void free_path(struct path *p)
{
        struct path *t;
        while (p) {
                t = p;
                p = p->next;
                free(t);
        }
}

void free_door(struct door *d)
{
        struct door *t;
        while (d) {
                t = d;
                d = d->next;
                free(t);
        }
}

void show_path(const struct path *p)
{
        for ( ; p; p = p->next)
                mvwaddch(gamew, p->pos.y, p->pos.x, path_symb);
        wrefresh(gamew);
}
