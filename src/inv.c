/* inv.c - inventory */

#include "rogue.h"

void show_inv(struct loot_list *inv)
{
        int y = 0, x = 0;
        struct loot_list *t;
        wclear(invw);
        wattrset(invw, A_REVERSE);
        mvwprintw(invw, y, x, "Weapons");
        wattrset(invw, A_NORMAL);
        for (y++, t = inv; t; t = t->next) {
                if (t->l->type == type_weapon) {
                        mvwprintw(invw, y, x, t->l->name);
                        y++;
                }
        }
        wattrset(invw, A_REVERSE);
        mvwprintw(invw, y, x, "Armor");
        wattrset(invw, A_NORMAL);
        for (y++, t = inv; t; t = t->next) {
                if (t->l->type == type_armor) {
                        mvwprintw(invw, y, x, t->l->name);
                        y++;
                }
        }
        wattrset(invw, A_REVERSE);
        mvwprintw(invw, y, x, "Poitions");
        wattrset(invw, A_NORMAL);
        for (y++, t = inv; t; t = t->next) {
                if (t->l->type == type_poition) {
                        mvwprintw(invw, y, x, t->l->name);
                        y++;
                }
        }
        y++;
        wattrset(invw, A_REVERSE);
        mvwprintw(invw, y, x, "(end)");
        wattrset(invw, A_NORMAL);
        wrefresh(invw);
        wait_ch(' ');
}
