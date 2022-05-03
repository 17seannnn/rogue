/* hunter.c - hunter control */

#include <curses.h>

#include "rogue.h"

static const char msg_no_endpoint[] = "No endpoint here...";
static const char msg_go_next_prompt[] = "Go next level? [yn]";
static const char msg_no_key[] = "You don't have a key.";

int is_hunter(const struct creature *h, int x, int y)
{
        return h->pos.x == x && h->pos.y == y;
}

void init_hunter(struct creature *h, const struct level *l)
{
	struct creature temp = {
		cast_hunter,
		"Hunter",
		'@', hunter_pair,
		{ COLOR_MAGENTA+8, -1 },
		{ l->start.pos.x, l->start.pos.y },
		0, 10, 1,
		0, 0, 0,
		NULL, NULL,
		NULL,
		{ 0, 0, 0 }, { 0, 0, 0 },
		0
	};
	set_pair(hunter_pair, &temp.clr);
	temp.symb |= COLOR_PAIR(hunter_pair);
	*h = temp;
}

void show_info(const struct creature *h)
{
        wclear(infow);
        mvwprintw(infow, 0, 0,
                  "HP:%d Dmg:%d Bld:%d Exp %d/%d        ",
                  h->hp, h->dmg, h->blood, h->exp, (int)power(2, h->level));
        wrefresh(infow);
}

void try_move(struct level *l, struct creature *h, int c, unsigned *flags)
{
        int side, x, y, res;
        side = get_char_side(c);
        get_side_diff(side, &x, &y);
        x += h->pos.x;
        y += h->pos.y;
        try_loot(l, h, side);
        res = move_creature(l, h, h, side);
        if (!res)
                try_attack_beast(h, l->b, side);
        if (c >= 'A' && c <= 'Z') {
                if (!res || is_stop(l, x, y)) {
                        if (*flags & again_flag)
                                *flags ^= again_flag;
                } else {
                        *flags |= again_flag;
                }
        }
}

int try_attack_beast(struct creature *h, struct beast *b, int side)
{
        int x, y;
        get_side_diff(side, &x, &y);
        x += h->pos.x;
        y += h->pos.y;
        for ( ; b; b = b->next)
                if (b->c.pos.x == x && b->c.pos.y == y)
                        return attack(h, &b->c);
        return 0;
}

void handle_hunter(struct creature *h)
{
        int level_exp = (int)power(2, h->level);
        if (h->exp >= level_exp) {
                h->exp -= level_exp;
                h->level++;
        }
}

void go_next(const struct level *l, struct creature *h, unsigned *flags)
{
        int c;
        struct loot_list *t;
        if (h->pos.x != l->end.pos.x || h->pos.y != l->end.pos.y) {
                add_msg(msg_no_endpoint);
                return;
        }
        for (t = h->inv; t && t->l->type != type_key; t = t->next)
                {}
        if (!t) {
                add_msg(msg_no_key);
                return;
        }
        add_msg(msg_go_next_prompt);
        handle_msg();
        do
                c = wgetch(gamew);
        while (c != 'Y' && c != 'y' &&
               c != 'N' && c != 'n' && c != key_escape);
        switch (c) {
        case key_escape:
                add_msg(msg_nevermind);
                return;
        case 'Y':
        case 'y':
                *flags |= next_level_flag;
                del_loot(&h->inv, t);
        }
}
