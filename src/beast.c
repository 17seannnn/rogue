/* beast.c - control beasts */

#include <stdlib.h>

#include "rogue.h"

const struct creature beast_list[] = {
	{
		cast_beast,
		"Debug1",
		'1', beast_debug1_pair,
		{ COLOR_GREEN, -1 },
		{ 0, 0 },
		5, 10, 1,
		10, 1, 0,
		NULL, NULL,
		NULL,
		{ 0, 0, 0 }, { 0, 0, 0, 0 },
		0
	},
	{
		cast_beast,
		"Debug2",
		'2', beast_debug2_pair,
		{ COLOR_BLUE, -1 },
		{ 0, 0 },
		6, 15, 2,
		20, 2, 0,
		NULL, NULL,
		NULL,
		{ 0, 0, 0 }, { 0, 0, 0, 0 },
		0
	},
	{
		cast_beast,
		"Debug3",
		'3', beast_debug3_pair,
		{ COLOR_RED, -1 },
		{ 0, 0 },
		7, 20, 3,
		50, 4, 0,
		NULL, NULL,
		NULL,
		{ 0, 0, 0 }, { 0, 0, 0, 0 },
		0
	},
	{
		cast_beast,
		"Debug4",
		'4', beast_debug4_pair,
		{ COLOR_YELLOW, -1 },
		{ 0, 0 },
		10, 50, 2,
		100, 8, 0,
		NULL, NULL,
		NULL,
		{ 0, 0, 0 }, { 0, 0, 0, 0 },
		0
	}
};

static void add_beast(struct beast **b, const struct creature *c, int x, int y)
{
        struct beast *t;
        t = malloc(sizeof(*t));
	t->c = *c;
	t->c.pos.x = x;
	t->c.pos.y = y;
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

static const struct creature *rand_beast(const struct level *l,
                                         const struct creature *h)
{
	int power, chance;
	for (power = 0; power < 3; power++) {
		chance = l->lt->beast_chance[power+1] +
		         h->buff_beast_chance[power+1];
		if (chance < 0)
			break;
		else if (chance > 100)
			continue;
		if (rand() % 100 >= chance)
			break;
	}
	return &beast_list[l->lt->location*3 + power];
}

static void spawn_beast(struct level *l, const struct creature *h,
                        const struct room *r, int first)
{
	int x, y;
	int count, chance;
	const struct creature *b;
        chance = l->lt->beast_chance[0] + h->buff_beast_chance[0];
	if (chance < 0)
		chance = 0;
	else if (chance > 100)
		chance = 100;
        if (first)
                chance /= 3;
        for (count = 0; rand() % 100 < chance; count++) {
		if (count >= l->lt->max_beast_count || !can_place(l, h, x, y))
			break;
		x = r->tl.x + 1 + rand() % (room_len(r, 'h') - 2);
		y = r->tl.y + 1 + rand() % (room_len(r, 'v') - 2);
		b = rand_beast(l, h);
		add_beast(&l->b, b, x, y);
        }
}

void init_beast(struct level *l, const struct creature *h)
{
        int ch, no;
        struct room *r;
        l->b = NULL;
        for (ch = 'A'; ch <= 'D'; ch++) {
                for (no = 1; no <= 4; no++) {
                        r = get_room_by_idx(l->r, ch, no);
                        if (r)
                                spawn_beast(l, h, r, ch == 'A' && no == 1);
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

int count_beast(const struct beast *b)
{
        int i;
        for (i = 0; b; b = b->next, i++)
                {}
        return i;
}

struct beast *get_random_beast(const struct beast *b)
{
        int i;
        for (i = rand() % count_beast(b); i && b; b = b->next, i--)
                {}
        return (struct beast *)b;
}

static void del_beast(struct level *l, struct beast *del)
{
	struct beast **b = &l->b;
	struct loot_list *inv;
        for ( ; *b && *b != del; b = &(*b)->next)
                {}
        if (!*b)
                return;
	for (inv = del->c.inv; inv; inv = inv->next)
		add_loot(&l->l, inv->l, del->c.pos.x, del->c.pos.y, 0);
	free_loot(del->c.inv);
        *b = (*b)->next;
        free(del);
}

static void try_attack_hunter(struct creature *b, struct creature *h, int side)
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
                        del_beast(l, b);
                        continue;
                }
                side = search_creature(l, c, h);
                side = try_side(l, side, c->pos.x, c->pos.y);
                res = move_creature(l, h, c, side);
                if (!res)
                        try_attack_hunter(c, h, side);
        }
}
