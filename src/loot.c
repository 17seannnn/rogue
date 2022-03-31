/* loot.c - control loot */

#include <stdlib.h>

#include "rogue.h"

const struct loot weapon_list[] = {
        { "Debug weapon", type_weapon, 1 }
};
const struct loot armor_list[] = {
        { "Debug armor", type_armor, 5 }
};
const struct loot poition_list[] = {
        { "Debug poition", type_poition, 50 }
};
const struct loot key_list[] = {
        { "Debug level key", type_key, type_key_level }
};

static const char msg_picked_up[] = "You picked up ";
static const char msg_no_space[] = "You have no space in inventory.";

static int has_idx(struct loot_list *l, int idx)
{
        for ( ; l; l = l->next)
                if (l->idx == idx)
                        return 1;
        return 0;
}

void add_loot(struct loot_list **ll, const struct loot *l,
              int x, int y, unsigned flags)
{
        int idx;
        struct loot_list *t;
        for (idx = 'a'; has_idx(*ll, idx); idx++)
                {}
        t = malloc(sizeof(*t));
        t->idx   = idx;
        t->pos.x = x;
        t->pos.y = y;
        t->l     = l;
        for ( ; *ll && (*ll)->l->type < l->type; ll = &(*ll)->next)
                {}
        for ( ; *ll && (*ll)->l->type == l->type && (*ll)->idx < t->idx;
             ll = &(*ll)->next)
                {}
        t->next  = *ll;
        t->flags = flags;
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
                        if (r && no == 1)
                                add_loot(&l->l, &key_list[poition_debug],
                                         r->tl.x+1, r->tl.y+2, 0);
                        else if (r)
                                add_loot(&l->l, &poition_list[poition_debug],
                                         r->tl.x+1, r->tl.y+2, 0);
                }
        }
}

void free_loot(struct loot_list *l)
{
        struct loot_list *t;
        while (l) {
                t = l;
                l = l->next;
                free(t);
        }
}

void show_loot(const struct loot_list *l)
{
        for ( ; l; l = l->next)
                mvwaddch(gamew, l->pos.y, l->pos.x, loot_symb);
}

struct loot_list *get_loot_by_coord(const struct loot_list *l, int x, int y)
{
        for ( ; l; l = l->next)
                if (l->pos.x == x && l->pos.y == y)
                        return (struct loot_list *)l;
        return NULL;
}

void try_loot(struct level *l, struct creature *h, int side)
{
        int count, x, y;
        struct loot_list *ll;
        for (count = 0, ll = h->inv; ll; ll = ll->next, count++)
                {}
        get_side_diff(side, &x, &y);
        x += h->pos.x;
        y += h->pos.y;
        if (is_beast(l->b, x, y))
                return;
        ll = get_loot_by_coord(l->l, x, y);
        if (!ll)
                return;
        if (count == max_inv) {
                add_msg(msg_no_space);
                return;
        }
        add_msg(msg_picked_up);
        append_msg(ll->l->name);
        append_msg(".");
        add_loot(&h->inv, ll->l, 0, 0, 0);
        del_loot(&l->l, ll);
}
