/* inv.c - inventory */

#include "rogue.h"

void show_inv(struct creature *h)
{
        int y = 0, x = 0;
        struct loot_list *t;
        wclear(invw);
        wattrset(invw, A_REVERSE);
        mvwprintw(invw, y, x, "Weapons");
        wattrset(invw, A_NORMAL);
        for (y++, t = h->inv; t; t = t->next) {
                if (t->l->type == type_weapon) {
                        mvwprintw(invw, y, x, "%c - %s %s",
                                  t->idx, t->l->name,
                                  h->weapon == t ? "(being worn)" : "");
                        y++;
                }
        }
        wattrset(invw, A_REVERSE);
        mvwprintw(invw, y, x, "Armor");
        wattrset(invw, A_NORMAL);
        for (y++, t = h->inv; t; t = t->next) {
                if (t->l->type == type_armor) {
                        mvwprintw(invw, y, x, "%c - %s %s",
                                  t->idx, t->l->name,
                                  h->armor == t ? "(being worn)" : "");
                        y++;
                }
        }
        wattrset(invw, A_REVERSE);
        mvwprintw(invw, y, x, "Poitions");
        wattrset(invw, A_NORMAL);
        for (y++, t = h->inv; t; t = t->next) {
                if (t->l->type == type_poition) {
                        mvwprintw(invw, y, x, "%c - %s", t->idx, t->l->name);
                        y++;
                }
        }
        wattrset(invw, A_REVERSE);
        mvwprintw(invw, y, x, "Key");
        wattrset(invw, A_NORMAL);
        for (y++, t = h->inv; t; t = t->next) {
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

void drop(struct loot_list **ll, struct creature *h)
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
                case key_escape:
                        add_msg("Never mind.");
                        return;
                case '*':
                        add_msg("You are checking inventory...");
                        handle_msg();
                        show_inv(h);
                        break;
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
                append_msg("' can't be dropped.");
                return;
        }
        add_msg("You dropped ");
        append_msg(t->l->name);
        append_msg(".");
        if (h->weapon == t)
                h->weapon = NULL;
        add_loot(ll, t->l, h->pos.x, h->pos.y);
        del_loot(&h->inv, t);
}

void wield(struct creature *h)
{
        int idx, got;
        char buf[] = " ";
        struct loot_list *t;
        for (got = 0; !got; ) {
                add_msg("What do you want to wield? [");
                for (t = h->inv; t; t = t->next) {
                        if (t->l->type == type_weapon) {
                                buf[0] = t->idx;
                                append_msg(buf);
                        }
                }
                append_msg(" or *]");
                handle_msg();
                idx = wgetch(gamew);
                switch (idx) {
                case key_escape:
                        add_msg("Never mind.");
                        return;
                case '*':
                        add_msg("You are checking inventory...");
                        handle_msg();
                        show_inv(h);
                        break;
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
                append_msg("' can't be wielded.");
                return;
        }
        add_msg("You wield ");
        append_msg(t->l->name);
        append_msg(".");
        h->weapon = t;
}
