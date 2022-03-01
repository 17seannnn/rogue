#include <stdlib.h>

#include "rogue.h"

static void add_beast(struct beast **b, int x, int y)
{
        struct beast *t;
        t = malloc(sizeof(*t));
        t->symb = 'B';
        t->pos.x = x;
        t->pos.y = y;
        t->hp = 10;
        t->dmg = 1;
        t->next = *b;
        *b = t;
}

int is_beast(struct beast *b, int x, int y)
{
        for ( ; b; b = b->next)
                if (b->pos.x == x && b->pos.y == y)
                        return 1;
        return 0;
}

void init_beast(struct level *l, const struct hunter *h)
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

void handle_beast(const struct level *l, struct hunter *h)
{

}

void show_beast(const struct beast *b)
{
        for ( ; b; b = b->next)
                mvwaddch(gamew, b->pos.y, b->pos.x, b->symb);
}
