/* hunter.c - hunter control */

#include <curses.h>

#include "rogue.h"

enum {
        hunter_symb = '@'
};

void init_hunter(struct hunter *h, struct level *l)
{
        h->symb = hunter_symb;
        h->pos.x = l->start.x;
        h->pos.y = l->start.y;
}

void show_hunter(const struct hunter *h)
{
        mvwaddch(gamew, h->pos.y, h->pos.x, h->symb);
        wrefresh(gamew);
}
