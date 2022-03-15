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
                        mvwprintw(invw, y, x, "%c - %s", t->idx, t->l->name);
                        y++;
                }
        }
        wattrset(invw, A_REVERSE);
        mvwprintw(invw, y, x, "Armor");
        wattrset(invw, A_NORMAL);
        for (y++, t = inv; t; t = t->next) {
                if (t->l->type == type_armor) {
                        mvwprintw(invw, y, x, "%c - %s", t->idx, t->l->name);
                        y++;
                }
        }
        wattrset(invw, A_REVERSE);
        mvwprintw(invw, y, x, "Poitions");
        wattrset(invw, A_NORMAL);
        for (y++, t = inv; t; t = t->next) {
                if (t->l->type == type_poition) {
                        mvwprintw(invw, y, x, "%c - %s", t->idx, t->l->name);
                        y++;
                }
        }
        wattrset(invw, A_REVERSE);
        mvwprintw(invw, y, x, "Key");
        wattrset(invw, A_NORMAL);
        for (y++, t = inv; t; t = t->next) {
                if (t->l->type == type_key) {
                        mvwprintw(invw, y, x, "%c - %s", t->idx, t->l->name);
                        y++;
                        break;
                }
        }
        y++;
        wattrset(invw, A_REVERSE);
        mvwprintw(invw, y, x, "(end)");
        wattrset(invw, A_NORMAL);
        wrefresh(invw);
        wait_ch(' ');
        /* TODO redraw screen */
}

void drop_inv(struct loot_list **ll, struct creature *h)
{
        int idx, got;
        char buf[] = " ";
        struct loot_list *t;
        for (got = 0; !got; ) {
                add_msg("What do you want to drop? [");
                for (t = h->inv; t; t = t->next) {
                        buf[0] = t->idx;
                        append_msg(buf);
                }
                append_msg(" or *]");
                handle_msg();
                idx = wgetch(gamew);
                switch (idx) {
                case '*':
                        show_inv(h->inv);
                        break;
                case key_escape:
                        add_msg("Never mind.");
                        return;
                default:
                        got = 1;
                        break;
                }
        }
        for (t = h->inv; t && t->idx != idx; t = t->next)
                {}
        if (!t) {
                add_msg("'");
                buf[0] = idx;
                append_msg(buf);
                append_msg("' can`t be dropped.");
                return;
        }
        add_msg("You dropped ");
        append_msg(t->l->name);
        append_msg(".");
        add_loot(ll, t->l, h->pos.x, h->pos.y);
        del_loot(&h->inv, t);
}
