/* beast.c - control beasts */

#include <stdlib.h>

#include "rogue.h"

static void add_beast(struct beast **b, int x, int y)
{
        struct beast *t;
        t = malloc(sizeof(*t));
        t->c.symb = 'B';
        t->c.pos.x = x;
        t->c.pos.y = y;
        t->c.hp = 10;
        t->c.dmg = 1;
        t->c.fov = 5;
        t->c.cast = cast_beast;
        t->next = *b;
        *b = t;
}

int is_beast(const struct beast *b, int x, int y)
{
        for ( ; b; b = b->next)
                if (b->c.pos.x == x && b->c.pos.y == y)
                        return 1;
        return 0;
}

void init_beast(struct level *l, const struct creature *h)
{
        int ch, no;
        struct room *r;
        for (ch = 'A'; ch <= 'D'; ch++) {
                for (no = 1; no <= 4; no++) {
                        r = get_room_by_idx(l->r, ch, no);
                        if (r)
                                add_beast(&l->b, r->tl.x+1, r->tl.y+1);
                }
        }
}

void free_beast(struct beast *b)
{
        struct beast *t;
        while (b) {
                t = b;
                b = b->next;
                free(t);
        }
}

void handle_beast(struct level *l, struct creature *h)
{
        int side;
        struct beast *b;
        struct creature *c;
        for (b = l->b; b; b = b->next) {
                c = &b->c;
                side = search_creature(c, h);
                side = try_side(l, side, c->pos.x, c->pos.y);
                move_creature(l, h, c, side);
        }
}
