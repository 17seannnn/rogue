/* hunter.c - hunter control */

#include <curses.h>

#include "rogue.h"

enum {
        hunter_symb = '@'
};

void init_hunter(struct hunter *h)
{
        h->symb = hunter_symb;
        h->cur_x = 0;
        h->cur_y = 0;
}

void show_hunter(const struct hunter *h)
{
        mvwaddch(gamew, h->cur_y, h->cur_x, h->symb);
        wrefresh(gamew);
}
