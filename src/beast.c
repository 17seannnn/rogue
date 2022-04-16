/* beast.c - control beasts */

#include <stdlib.h>

#include "rogue.h"

static void add_beast(struct beast **b, int x, int y)
{
        struct beast *t;
        t = malloc(sizeof(*t));
        t->c.flags  = 0;
        t->c.cast   = cast_beast;
        t->c.symb   = 'B';
        t->c.pos.x  = x;
        t->c.pos.y  = y;
        t->c.hp     = 10;
        t->c.dmg    = 1;
        t->c.fov    = 5;
        t->c.blood  = 0;
        t->c.exp    = 1;
        t->c.level  = 1;
        t->c.weapon = NULL;
        t->c.armor  = NULL;
        t->c.inv    = NULL;
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

static int can_place(const struct level *l, const struct creature *h,
                     int x, int y)
{
        return !is_beast(l->b, x, y) && !is_hunter(h, x, y);
}

void init_beast(struct level *l, const struct creature *h)
{
        int ch, no, x, y;
        struct room *r;
        l->b = NULL;
        for (ch = 'A'; ch <= 'D'; ch++) {
                for (no = 1; no <= 4; no++) {
                        r = get_room_by_idx(l->r, ch, no);
                        if (r) {
                                while (rand() % 100 < l->lt->beast_chance) {
                                        x = r->tl.x + 1 + rand() %
                                            (room_len(r, 'h') - 2);
                                        y = r->tl.y + 1 + rand() %
                                            (room_len(r, 'v') - 2);
                                        if (can_place(l, h, x, y))
                                                add_beast(&l->b, x, y);
                                        else
                                                break;
                                }
                        }
                }
        }
}

void free_beast(struct beast *b)
{
        struct beast *t;
        while (b) {
                free_loot(b->c.inv);
                t = b;
                b = b->next;
                free(t);
        }
}

static void del_beast(struct beast **b, struct beast *del)
{
        for ( ; *b && *b != del; b = &(*b)->next)
                {}
        if (!*b)
                return;
        *b = (*b)->next;
        free(del);
}

static void try_attack_hunter(const struct creature *b, struct creature *h,
                                                        int side)
{
        int x, y;
        get_side_diff(side, &x, &y);
        x += b->pos.x;
        y += b->pos.y;
        if (is_hunter(h, x, y))
                attack(b, h);
}

void handle_beast(struct level *l, struct creature *h)
{
        int res, side;
        struct beast *b;
        struct creature *c;
        for (b = l->b; b; b = b->next) {
                c = &b->c;
                if (c->hp <= 0) {
                        del_beast(&l->b, b);
                        continue;
                }
                side = search_creature(l, c, h);
                side = try_side(l, side, c->pos.x, c->pos.y);
                res = move_creature(l, h, c, side);
                if (!res)
                        try_attack_hunter(c, h, side);
        }
}
