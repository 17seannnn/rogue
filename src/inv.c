/* inv.c - inventory */

#include "rogue.h"

void show_inv(struct inventory *inv)
{
        int y = 0, x = 0;
        struct inventory *t;
        wclear(invw);
        mvwprintw(invw, y, x, "Weapons");
        for (t = inv; t; t = t->next, y++)
                if (t->l->type == type_weapon)
                        mvwprintw(invw, y, x, t->l->name);
        y++;
        mvwprintw(invw, y, x, "Armor");
        for (t = inv; t; t = t->next, y++)
                if (t->l->type == type_armor)
                        mvwprintw(invw, y, x, t->l->name);
        y++;
        mvwprintw(invw, y, x, "Poitions");
        for (t = inv; t; t = t->next, y++)
                if (t->l->type == type_poition)
                        mvwprintw(invw, y, x, t->l->name);
        y++;
        mvwprintw(invw, y, x, "(end)");
        wrefresh(invw);
        wait_ch(' ');
}
