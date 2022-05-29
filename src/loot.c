/* loot.c - control loot */

#include <stdio.h>
#include <stdlib.h>

#include "rogue.h"

const struct loot blood_list[] = {
        { "Small bag of blood", type_blood, 2 },
        { "Bag of blood", type_blood, 4 },
        { "Empty", 0, 0 },

        { "Small bag of blood", type_blood, 4 },
        { "Bag of blood", type_blood, 8 },
        { "Empty", 0, 0 },

        { "Small bag of blood", type_blood, 8 },
        { "Bag of blood", type_blood, 16 },
        { "Big bag of blood", type_blood, 32 },

        { "Small bag of blood", type_blood, 16 },
        { "Bag of blood", type_blood, 32 },
        { "Big bag of blood", type_blood, 48 },

        { "Small bag of blood", type_blood, 32 },
        { "Bag of blood", type_blood, 48 },
        { "Big bag of blood", type_blood, 64 }
};

const struct loot weapon_list[] = {
        { "Broken scissors", type_weapon, 1 },
	{ "Dull knife", type_weapon, 2 },
	{ "Empty", 0, 0 },

        { "Scissors", type_weapon, 3 },
	{ "Knife", type_weapon, 4 },
	{ "Empty", 0, 0 },

        { "Big scissors", type_weapon, 4 },
	{ "Long knife", type_weapon, 6 },
	{ "Short sword", type_weapon, 8 },

        { "Sword", type_weapon, 10 },
	{ "Long sword", type_weapon, 12 },
	{ "Axe", type_weapon, 16 },

        { "Two-handed sword", type_weapon, 18 },
	{ "Sharpened ax", type_weapon, 24 },
	{ "Hammer", type_weapon, 32 }
};

const struct loot armor_list[] = {
        { "Light armor", type_armor, 10 },
        { "Default armor", type_armor, 25 },
        { "Empty", 0, 0 },

        { "Light armor", type_armor, 10 },
        { "Default armor", type_armor, 25 },
        { "Empty", 0, 0 },

        { "Light armor", type_armor, 10 },
        { "Default armor", type_armor, 25 },
        { "Heavy armor", type_armor, 50 },

        { "Light armor", type_armor, 10 },
        { "Default armor", type_armor, 25 },
        { "Heavy armor", type_armor, 50 },

        { "Old armor", type_armor, 25 },
        { "Knight armor", type_armor, 50 },
        { "Metal armor", type_armor, 75 }
};

const struct loot potion_list[] = {
        { "Small potion", type_potion, 2 },
        { "Potion", type_potion, 4 },
        { "Empty", 0, 0 },

        { "Small potion", type_potion, 4 },
        { "Potion", type_potion, 8 },
        { "Empty", 0, 0 },

        { "Small potion", type_potion, 4 },
        { "Potion", type_potion, 8 },
        { "Big potion", type_potion, 16 },

        { "Small potion", type_potion, 8 },
        { "Potion", type_potion, 16 },
        { "Big potion", type_potion, 24 },

        { "Small potion", type_potion, 16 },
        { "Potion", type_potion, 24 },
        { "Big potion", type_potion, 32 }
};

const struct loot key_list[] = {
        { "Level key", type_key, type_key_level }
};

static const char msg_picked_up[] = "You picked up %s (%d).";
static const char msg_picked_up_key[] = "You picked up %s.";
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
        t->l     = *l;
        for ( ; *ll && (*ll)->l.type < l->type; ll = &(*ll)->next)
                {}
        for ( ; *ll && (*ll)->l.type == l->type && (*ll)->idx < t->idx;
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

static const struct loot *rand_loot(const struct level *l,
                                    const struct creature *h)
{
	/* location * 3 because we have 3 quality */
	int idx = l->lt->location*3 + rand_quality(l, h);
	switch (rand() % 4) {
	case 0: return &blood_list[idx];
	case 1: return &weapon_list[idx];
	case 2: return &armor_list[idx];
	case 3: return &potion_list[idx];
	default: return NULL;
	}
}

static void spawn_loot(struct level *l,
		       const struct creature *h,
		       struct room *r)
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
                add_loot(&l->l, &key_list[key_level], x, y, 0);
        } else {
                b = get_random_beast(l->b);
                add_loot(&b->c.inv, &key_list[key_level], 0, 0, 0);
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

int count_loot(const struct loot_list *ll)
{
	int count;
	for (count = 0; ll; ll = ll->next, count++)
		{}
	return count;
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
	if (ll->l.type == type_blood) {
		add_blood(l, h, ll->l.val);
	} else if (count_loot(h->inv) < max_inv) {
		add_loot(&h->inv, &ll->l, 0, 0, 0);
	} else {
                add_msg(msg_no_space);
                return;
	}
	if (ll->l.type == type_key)
		add_msg(msg_picked_up_key, ll->l.name);
	else
		add_msg(msg_picked_up, ll->l.name, ll->l.val);
        del_loot(&l->l, ll);
}
