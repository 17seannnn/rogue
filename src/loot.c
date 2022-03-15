/* loot.c - control loot */

#include <stdlib.h>

#include "rogue.h"

enum {
        loot_symb = ':'
};

const struct loot weapon_list[] = {
        { "Debug weapon", type_weapon, 1, 0 }
};

static const char msg_pickup[] = "You picked up ";

void add_loot(struct loot_list **ll, const struct loot *l, int x, int y)
{
        int idx;
        struct loot_list *t;
        t = malloc(sizeof(*t));
        t->pos.x = x;
        t->pos.y = y;
        t->l = l;
        for (idx = 'a'; *ll; ll = &(*ll)->next, idx++)
                {}
        t->idx = idx;
        t->next = NULL;
        *ll = t;
}

void del_loot(struct loot_list **l, struct loot_list *del)
{
        for ( ; *l; l = &(*l)->next) {
                if (*l == del) {
                        *l = (*l)->next;
                        free(del);
                        return;
                }
        }
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
                                add_loot(&l->l, &weapon_list[weapon_debug],
                                         r->tl.x+1, r->tl.y+2);
                }
        }
}

void show_loot(struct loot_list *l)
{
        for ( ; l; l = l->next)
                mvwaddch(gamew, l->pos.y, l->pos.x, loot_symb);
}

struct loot_list *get_loot_by_coord(struct loot_list *l, int x, int y)
{
        for ( ; l; l = l->next)
                if (l->pos.x == x && l->pos.y == y)
                        return l;
        return NULL;
}

void try_loot(struct level *l, struct creature *h, int side)
{
        int x, y;
        struct loot_list *ll;
        get_side_diff(side, &x, &y);
        x += h->pos.x;
        y += h->pos.y;
        if (is_beast(l->b, x, y))
                return;
        ll = get_loot_by_coord(l->l, x, y);
        if (!ll)
                return;
        add_loot(&h->inv, ll->l, 0, 0);
        del_loot(&l->l, ll);
        add_msg(msg_pickup);
        append_msg(h->inv->l->name);
        append_msg(".");
}
