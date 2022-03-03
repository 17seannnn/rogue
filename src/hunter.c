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

void init_hunter(struct creature *h, struct level *l)
{
        h->symb = hunter_symb;
        h->pos.x = l->start.x;
        h->pos.y = l->start.y;
        h->hp = 10;
        h->dmg = 1;
        h->fov = 0;
        h->cast = cast_hunter;
}

void show_info(const struct creature *h)
{
        mvwprintw(infow, 0, 0, "HP:%d Dmg:%d", h->hp, h->dmg);
        wrefresh(infow);
}
