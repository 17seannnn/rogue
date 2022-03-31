/* inv.c - inventory */

#include "rogue.h"

static const char msg_checking_inv[] = "You are checking inventory...";
static const char msg_or_inv[] = " or *]";

static const char msg_todrop[] = "What do you want to drop? [";
static const char msg_dropped[] = "You dropped ";
static const char msg_not_dropped[] = "' can't be dropped.";

static const char msg_towield[] = "What do you want to wield? [";
static const char msg_wielding[] = "Now you are wielding ";
static const char msg_not_worn[] = "' can't be worn.";

static const char msg_towear[] = "What do you want to wear? [";
static const char msg_wearing[] = "Now you are wearing ";

static const char msg_toquaff[] = "What do you want to quaff? [";
static const char msg_quaffed[] = "You quaffed  ";
static const char msg_not_quaffed[] = " can't be quaffed.";

void show_inv(const struct creature *h)
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
        mvwprintw(invw, y, x, "Keys");
        wattrset(invw, A_NORMAL);
        for (y++, t = h->inv; t; t = t->next) {
                if (t->l->type == type_key) {
                        mvwprintw(invw, y, x, "%c - %s", t->idx, t->l->name);
                        y++;
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
                add_msg(msg_todrop);
                for (t = h->inv; t; t = t->next) {
                        buf[0] = t->idx;
                        append_msg(buf);
                }
                append_msg(msg_or_inv);
                handle_msg();
                idx = wgetch(gamew);
                switch (idx) {
                case key_escape:
                        add_msg(msg_nevermind);
                        return;
                case '*':
                        add_msg(msg_checking_inv);
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
                append_msg(msg_not_dropped);
                return;
        }
        add_msg(msg_dropped);
        append_msg(t->l->name);
        append_msg(".");
        if (h->weapon == t)
                h->weapon = NULL;
        else
        if (h->armor == t)
                h->armor = NULL;
        add_loot(ll, t->l, h->pos.x, h->pos.y);
        del_loot(&h->inv, t);
}

void wield(struct creature *h)
{
        int idx, got;
        char buf[] = " ";
        struct loot_list *t;
        for (got = 0; !got; ) {
                add_msg(msg_towield);
                for (t = h->inv; t; t = t->next) {
                        if (t->l->type == type_weapon) {
                                buf[0] = t->idx;
                                append_msg(buf);
                        }
                }
                append_msg(msg_or_inv);
                handle_msg();
                idx = wgetch(gamew);
                switch (idx) {
                case key_escape:
                        add_msg(msg_nevermind);
                        return;
                case '*':
                        add_msg(msg_checking_inv);
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
        if (!t || t->l->type != type_weapon) {
                add_msg("'");
                buf[0] = idx;
                append_msg(buf);
                append_msg(msg_not_worn);
                return;
        }
        add_msg(msg_wielding);
        append_msg(t->l->name);
        append_msg(".");
        h->weapon = t;
}

void wear(struct creature *h)
{
        int idx, got;
        char buf[] = " ";
        struct loot_list *t;
        for (got = 0; !got; ) {
                add_msg(msg_towear);
                for (t = h->inv; t; t = t->next) {
                        if (t->l->type == type_armor) {
                                buf[0] = t->idx;
                                append_msg(buf);
                        }
                }
                append_msg(msg_or_inv);
                handle_msg();
                idx = wgetch(gamew);
                switch (idx) {
                case key_escape:
                        add_msg(msg_nevermind);
                        return;
                case '*':
                        add_msg(msg_checking_inv);
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
        if (!t || t->l->type != type_armor) {
                add_msg("'");
                buf[0] = idx;
                append_msg(buf);
                append_msg(msg_not_worn);
                return;
        }
        add_msg(msg_wearing);
        append_msg(t->l->name);
        append_msg(".");
        h->armor = t;
}

void quaff(struct creature *h)
{
        int idx, got;
        char buf[] = " ";
        struct loot_list *t;
        for (got = 0; !got; ) {
                add_msg(msg_toquaff);
                for (t = h->inv; t; t = t->next) {
                        if (t->l->type == type_poition) {
                                buf[0] = t->idx;
                                append_msg(buf);
                        }
                }
                append_msg(msg_or_inv);
                handle_msg();
                idx = wgetch(gamew);
                switch (idx) {
                case key_escape:
                        add_msg(msg_nevermind);
                        return;
                case '*':
                        add_msg(msg_checking_inv);
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
        if (!t || t->l->type != type_poition) {
                add_msg("'");
                buf[0] = idx;
                append_msg(buf);
                append_msg(msg_not_quaffed);
                return;
        }
        add_msg(msg_quaffed);
        append_msg(t->l->name);
        append_msg(".");
        h->hp += t->l->val;
        if (h->hp > 100)
                h->hp = 100;
        del_loot(&h->inv, t);
}
