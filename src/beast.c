/* beast.c - control beasts */

#include <stdlib.h>

#include "rogue.h"

const struct creature beast_list[] = {
    {
        cast_beast,
        "Rat",
        'r', beast_first_pair,
        { COLOR_YELLOW, -1 },
        { 0, 0 },
        3, 3, 3, 1, 33,
        5, 1, 0,
        NULL, NULL,
        NULL,
        { 0, 0, 0 }, { 0, 0, 0, 0 },
        0
    },
    {
        cast_beast,
        "Dog",
        'd', beast_second_pair,
        { COLOR_BLUE, -1 },
        { 0, 0 },
        5, 6, 6, 2, 33,
        10, 2, 0,
        NULL, NULL,
        NULL,
        { 0, 0, 0 }, { 0, 0, 0, 0 },
        0
    },
    {
        cast_beast,
        "Empty",
        ' ', beast_empty_pair,
        { -1, -1 },
        { 0, 0 },
        0, 0, 0, 0, 0,
        0, 0, 0,
        NULL, NULL,
        NULL,
        { 0, 0, 0 }, { 0, 0, 0, 0 },
        0
    },
    {
        cast_beast,
        "Empty",
        ' ', beast_empty_pair,
        { -1, -1 },
        { 0, 0 },
        0, 0, 0, 0, 0,
        0, 0, 0,
        NULL, NULL,
        NULL,
        { 0, 0, 0 }, { 0, 0, 0, 0 },
        0
    },

    {
        cast_beast,
        "Rat",
        'r', beast_first_pair,
        { COLOR_YELLOW, -1 },
        { 0, 0 },
        4, 3, 3, 1, 33,
        5, 2, 0,
        NULL, NULL,
        NULL,
        { 0, 0, 0 }, { 0, 0, 0, 0 },
        0
    },
    {
        cast_beast,
        "Dog",
        'd', beast_second_pair,
        { COLOR_BLUE, -1 },
        { 0, 0 },
        5, 6, 6, 2, 33,
        10, 3, 0,
        NULL, NULL,
        NULL,
        { 0, 0, 0 }, { 0, 0, 0, 0 },
        0
    },
    {
        cast_beast,
        "Empty",
        ' ', beast_empty_pair,
        { -1, -1 },
        { 0, 0 },
        0, 0, 0, 0, 0,
        0, 0, 0,
        NULL, NULL,
        NULL,
        { 0, 0, 0 }, { 0, 0, 0, 0 },
        0
    },
    {
        cast_beast,
        "Empty",
        ' ', beast_empty_pair,
        { -1, -1 },
        { 0, 0 },
        0, 0, 0, 0, 0,
        0, 0, 0,
        NULL, NULL,
        NULL,
        { 0, 0, 0 }, { 0, 0, 0, 0 },
        0
    },

    {
        cast_beast,
        "Big rat",
        'r', beast_first_pair,
        { COLOR_RED, -1 },
        { 0, 0 },
        4, 8, 8, 4, 33,
        10, 4, 0,
        NULL, NULL,
        NULL,
        { 0, 0, 0 }, { 0, 0, 0, 0 },
        0
    },
    {
        cast_beast,
        "Wild dog",
        'd', beast_second_pair,
        { COLOR_CYAN, -1 },
        { 0, 0 },
        6, 10, 10, 6, 33,
        20, 8, 0,
        NULL, NULL,
        NULL,
        { 0, 0, 0 }, { 0, 0, 0, 0 },
        0
    },
    {
        cast_beast,
        "Knight",
        'K', beast_third_pair,
        { COLOR_BLACK, -1 },
        { 0, 0 },
        5, 6, 6, 8, 50,
        36, 12, 0,
        NULL, NULL,
        NULL,
        { 0, 0, 0 }, { 0, 0, 0, 0 },
        0
    },
    {
        cast_beast,
        "Empty",
        ' ', beast_empty_pair,
        { -1, -1 },
        { 0, 0 },
        0, 0, 0, 0, 0,
        0, 0, 0,
        NULL, NULL,
        NULL,
        { 0, 0, 0 }, { 0, 0, 0, 0 },
        0
    },

    {
        cast_beast,
        "Raven",
        'R', beast_first_pair,
        { COLOR_BLUE, -1 },
        { 0, 0 },
        6, 10, 10, 5, 33,
        15, 10, 0,
        NULL, NULL,
        NULL,
        { 0, 0, 0 }, { 0, 0, 0, 0 },
        0
    },
    {
        cast_beast,
        "Demon",
        'D', beast_second_pair,
        { COLOR_RED, -1 },
        { 0, 0 },
        5, 15, 15, 10, 33,
        40, 16, 0,
        NULL, NULL,
        NULL,
        { 0, 0, 0 }, { 0, 0, 0, 0 },
        0
    },
    {
        cast_beast,
        "Schizophrenic",
        'S', beast_third_pair,
        { COLOR_YELLOW, -1 },
        { 0, 0 },
        4, 10, 10, 12, 33,
        64, 32, 0,
        NULL, NULL,
        NULL,
        { 0, 0, 0 }, { 0, 0, 0, 0 },
        0
    },
    {
        cast_beast,
        "Empty",
        ' ', beast_empty_pair,
        { -1, -1 },
        { 0, 0 },
        0, 0, 0, 0, 0,
        0, 0, 0,
        NULL, NULL,
        NULL,
        { 0, 0, 0 }, { 0, 0, 0, 0 },
        0
    },

    {
        cast_beast,
        "Black raven",
        'R', beast_first_pair,
        { COLOR_BLACK, -1 },
        { 0, 0 },
        6, 12, 12, 8, 33,
        15, 16, 0,
        NULL, NULL,
        NULL,
        { 0, 0, 0 }, { 0, 0, 0, 0 },
        0
    },
    {
        cast_beast,
        "Fossegrim",
        'F', beast_second_pair,
        { COLOR_YELLOW, -1 },
        { 0, 0 },
        5, 20, 20, 15, 33,
        40, 24, 0,
        NULL, NULL,
        NULL,
        { 0, 0, 0 }, { 0, 0, 0, 0 },
        0
    },
    {
        cast_beast,
        "Nokk",
        'n', beast_third_pair,
        { COLOR_MAGENTA, -1 },
        { 0, 0 },
        4, 18, 18, 20, 33,
        64, 32, 0,
        NULL, NULL,
        NULL,
        { 0, 0, 0 }, { 0, 0, 0, 0 },
        0
    },
    {
        cast_beast,
        "Varulv",
        'v', beast_fourth_pair,
        { COLOR_WHITE, -1 },
        { 0, 0 },
        6, 24, 24, 25, 50,
        128, 128, 0,
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
    set_pair(t->c.color_pair, &t->c.clr);
    t->c.symb |= COLOR_PAIR(t->c.color_pair);
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
                                         const struct creature *h,
                                         int first)
{
    int power, chance;
    for (power = 0; power < 3; power++) {
        chance = l->lt->beast_chance[power+1] + h->buff_beast_chance[power+1];
        if (chance < 0)
            break;
        else if (chance > 100)
            continue;
        if (rand() % 100 >= chance)
            break;
    }
    if (first && power > 1)
        power = 1;
    return &beast_list[l->lt->location*4 + power];
}

static void spawn_beast(struct level *l, const struct creature *h,
                        const struct room *r, int first)
{
    int x, y;
    int count, chance;
    chance = l->lt->beast_chance[0] + h->buff_beast_chance[0];
    if (chance < 0)
        chance = 0;
    else if (chance > 100)
        chance = 100;
    if (first)
        chance /= 3;
    for (count = 0; rand() % 100 < chance; count++) {
        x = r->tl.x + 1 + rand() % (room_len(r, 'h') - 2);
        y = r->tl.y + 1 + rand() % (room_len(r, 'v') - 2);
        if (count >= l->lt->max_beast_count || !can_place(l, h, x, y))
            break;
        add_beast(&l->b, rand_beast(l, h, first), x, y);
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
        add_loot(&l->l, &inv->l, del->c.pos.x, del->c.pos.y, 0);
    free_loot(del->c.inv);
    *b = (*b)->next;
    free(del);
}

static void try_attack_hunter(struct level *l, struct creature *b,
                              struct creature *h, int side)
{
    int x, y;
    get_side_diff(side, &x, &y);
    x += b->pos.x;
    y += b->pos.y;
    if (is_hunter(h, x, y))
        attack(l, b, h);
}

void handle_beast(struct level *l, struct creature *h)
{
    int res, side;
    struct beast *b = l->b, *t;
    struct creature *c;
    while (b) {
        c = &b->c;
        if (c->hp <= 0) {
            t = b;
            b = b->next;
            del_beast(l, t);
            continue;
        }
        side = search_creature(l, c, h);
        side = try_side(l, side, c->pos.x, c->pos.y);
        res = move_creature(l, h, c, side);
        if (!res)
            try_attack_hunter(l, c, h, side);
        b = b->next;
    }
}
