/* hunter.c - hunter control */

#include <curses.h>

#include "rogue.h"

enum {
        hunter_symb = '@'
};

int is_hunter(const struct creature *h, int x, int y)
{
        return h->pos.x == x && h->pos.y == y;
}

void init_hunter(struct creature *h, const struct level *l)
{
        h->flags  = 0;
        h->cast   = cast_hunter;
        h->symb   = hunter_symb;
        h->pos.x  = l->start.x;
        h->pos.y  = l->start.y;
        h->hp     = 10;
        h->dmg    = 1;
        h->fov    = 0;
        h->blood  = 0;
        h->exp    = 0;
        h->level  = 0;
        h->weapon = NULL;
        h->armor  = NULL;
        h->inv    = NULL;
}

void show_info(const struct creature *h)
{
        wclear(infow);
        mvwprintw(infow, 0, 0,
                  "HP:%d Dmg:%d Bld:%d Exp %d/%d ",
                  h->hp, h->dmg, h->blood, h->exp, (int)power(2, h->level));
        wrefresh(infow);
}

void try_move(struct level *l, struct creature *h, int c, int *flags)
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
                if (!res || is_door(l->d, x, y)) {
                        if (*flags & again)
                                *flags ^= again;
                } else {
                        *flags |= again;
                }
        }
}

int try_attack_beast(const struct creature *h, struct beast *b, int side)
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
