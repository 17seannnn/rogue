/* loot.c - control loot */

#include <stdlib.h>

#include "rogue.h"

const struct loot weapon_list[] = {
        { "Debug weapon", type_weapon, 1, 0 }
};

static void add_loot(struct loot_list **l, int x, int y)
{
        struct loot_list *t;
        t = malloc(sizeof(*t));
        t->x = x;
        t->y = y;
        t->l = &weapon_list[weapon_debug];
        t->next = *l;
        *l = t;
}

void init_loot(struct level *l, const struct creature *h)
{
        int ch, no;
        struct room *r;
        l->l = NULL;
        for (ch = 'A'; ch <= 'D'; ch++) {
                for (no = 1; no <= 4; no++) {
                        r = get_room_by_idx(l->r, ch, no);
                        if (r)
                                add_loot(&l->l, r->tl.x+1, r->tl.y+2);
                }
        }
}
