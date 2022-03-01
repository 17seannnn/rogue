/* hunter.c - hunter control */

#include <curses.h>

#include "rogue.h"

enum {
        hunter_symb = '@'
};

int is_hunter(const struct hunter *h, int x, int y)
{
        return h->pos.x == x && h->pos.y == y;
}

void init_hunter(struct hunter *h, struct level *l)
{
        h->symb = hunter_symb;
        h->pos.x = l->start.x;
        h->pos.y = l->start.y;
        h->hp = 10;
        h->dmg = 1;
}

void show_hunter(const struct hunter *h)
{
        mvwaddch(gamew, h->pos.y, h->pos.x, h->symb);
        wrefresh(gamew);
}

void show_info(const struct hunter *h)
{
        mvwprintw(infow, 0, 0, "HP:%d Dmg:%d", h->hp, h->dmg);
        wrefresh(infow);
}
