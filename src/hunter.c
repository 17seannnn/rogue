/* hunter.c - hunter control */

#include <curses.h>

#include "rogue.h"

enum {
        hunter_symb = '@'
};

void init_hunter(struct hunter *h, struct level *l)
{
        h->symb = hunter_symb;
        h->cur_x = l->start.x;
        h->cur_y = l->start.y;
}

void show_hunter(const struct hunter *h)
{
        mvwaddch(gamew, h->cur_y, h->cur_x, h->symb);
        wrefresh(gamew);
}
