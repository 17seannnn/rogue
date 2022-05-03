/* loot.c - control loot */

#include <stdio.h>
#include <stdlib.h>

#include "rogue.h"

const struct loot blood_list[] = {
        { "Debug blood 1", type_blood, 50 },
        { "Debug blood 2", type_blood, 100 },
        { "Debug blood 3", type_blood, 150 }
};

const struct loot weapon_list[] = {
        { "Debug weapon 1", type_weapon, 1 },
	{ "Debug weapon 2", type_weapon, 2 },
	{ "Debug weapon 3", type_weapon, 3 }
};

const struct loot armor_list[] = {
        { "Debug armor 1", type_armor, 5 },
        { "Debug armor 2", type_armor, 10 },
        { "Debug armor 3", type_armor, 15 }
};

const struct loot poition_list[] = {
        { "Debug poition 1", type_poition, 25 },
        { "Debug poition 2", type_poition, 50 },
        { "Debug poition 3", type_poition, 100 }
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

struct loot_list *get_loot_by_coord(const struct loot_list *l, int x, int y)
{
        for ( ; l; l = l->next)
                if (l->pos.x == x && l->pos.y == y)
                        return (struct loot_list *)l;
        return NULL;
}

int is_loot(struct loot_list *l, int x, int y)
{
        struct loot_list *t = get_loot_by_coord(l, x, y);
        return t != NULL ? 1 : 0;
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

/* Return value from 0 to 2 and that should fit player needs*/
static int rand_quality(const struct level *l, const struct creature *h)
{
	int quality, chance;
	for (quality = 0; quality < 2; quality++) {
		chance = l->lt->loot_chance[quality+1] +
		         h->buff_loot_chance[quality+1];
		if (chance < 0)
			break;
		else if (chance > 100)
			continue;
		if (rand() % 100 >= chance)
			break;
	}
	return quality;
}

/* TODO: should check what player needs and give it */
static const struct loot *rand_loot(const struct level *l,
                                    const struct creature *h)
{
	/* location * 3 because we have 3 quality */
	int idx = l->lt->location*3 + rand_quality(l, h);
	switch (rand() % 4) {
	case 0: return &blood_list[idx];
	case 1: return &weapon_list[idx];
	case 2: return &armor_list[idx];
	case 3: return &poition_list[idx];
	default: return NULL;
	}
}

static void spawn_loot(struct level *l, const struct creature *h, struct room *r)
{
	int x, y;
        int count, chance;
        const struct loot *lp;
	chance = l->lt->loot_chance[0] + h->buff_loot_chance[0];
	if (chance < 0)
		chance = 0;
	else if (chance > 100)
		chance = 100;
        for (count = 0; rand() % 100 < chance; count++) {
                if (count >= l->lt->max_loot_count)
			break;
		x = r->tl.x + 1 + rand() % (room_len(r, 'h') - 2);
		y = r->tl.y + 1 + rand() % (room_len(r, 'v') - 2);
		lp = rand_loot(l, h);
		add_loot(&l->l, lp, x, y, 0);
        }
}

static void spawn_key(struct level *l)
{
        int x, y;
        struct room *r;
        struct beast *b;
        if (rand() % 2 || count_beast(l->b) == 0) {
                r = get_random_room(l->r);
                x = r->tl.x + 1 + rand() % (room_len(r, 'h') - 2);
                y = r->tl.y + 1 + rand() % (room_len(r, 'v') - 2);
                add_loot(&l->l, &key_list[key_debug], x, y, 0);
        } else {
                b = get_random_beast(l->b);
                add_loot(&b->c.inv, &key_list[key_debug], 0, 0, 0);
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
                                spawn_loot(l, h, r);
                }
        }
        spawn_key(l);
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

void try_loot(struct level *l, struct creature *h, int side)
{
        int count, x, y;
	char buf[bufsize];
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
	append_msg(" (");
	snprintf(buf, bufsize, "%d", ll->l->val);
	append_msg(buf);
        append_msg(").");
	if (ll->l->type == type_blood)
		h->blood += ll->l->val;
	else
		add_loot(&h->inv, ll->l, 0, 0, 0);
        del_loot(&l->l, ll);
}
