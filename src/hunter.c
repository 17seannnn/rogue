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
        mvwprintw(infow, 0, 0, "HP:%d Dmg:%d Bld:%d Exp %d/%d ",
                                h->hp, h->dmg, h->blood, h->exp, (int)power(2, h->level));
        wrefresh(infow);
}
