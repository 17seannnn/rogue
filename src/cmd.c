/* cmd.c - hunter`s cmd control */

#include "rogue.h"

static void do_move(struct hunter *h, int dx, int dy)
{
        h->pos.x += dx;
        h->pos.y += dy;
}

int do_cmd(int c, struct hunter *h, struct level *l)
{
        switch (c) {
        case 'h':
                if (can_move(l, h->pos.x-1, h->pos.y))
                        do_move(h, -1, 0);
                return 0;
        case 'H':
                if (can_move(l, h->pos.x-1, h->pos.y)) {
                        do_move(h, -1, 0);
                        return !is_door(l->d, h->pos.x, h->pos.y);
                }
                return 0;
        case 'j':
                if (can_move(l, h->pos.x, h->pos.y+1))
                        do_move(h, 0, 1);
                return 0;
        case 'J':
                if (can_move(l, h->pos.x, h->pos.y+1)) {
                        do_move(h, 0, 1);
                        return !is_door(l->d, h->pos.x, h->pos.y);
                }
                return 0;
        case 'k':
                if (can_move(l, h->pos.x, h->pos.y-1))
                        do_move(h, 0, -1);
                return 0;
        case 'K':
                if (can_move(l, h->pos.x, h->pos.y-1)) {
                        do_move(h, 0, -1);
                        return !is_door(l->d, h->pos.x, h->pos.y);
                }
                return 0;
        case 'l':
                if (can_move(l, h->pos.x+1, h->pos.y))
                        do_move(h, 1, 0);
                return 0;
        case 'L':
                if (can_move(l, h->pos.x+1, h->pos.y)) {
                        do_move(h, 1, 0);
                        return !is_door(l->d, h->pos.x, h->pos.y);
                }
                return 0;
        case 'y':
                if (can_move(l, h->pos.x-1, h->pos.y-1))
                        do_move(h, -1, -1);
                return 0;
        case 'Y':
                if (can_move(l, h->pos.x-1, h->pos.y-1)) {
                        do_move(h, -1, -1);
                        return !is_door(l->d, h->pos.x, h->pos.y);
                }
                return 0;
        case 'u':
                if (can_move(l, h->pos.x+1, h->pos.y-1))
                        do_move(h, 1, -1);
                return 0;
        case 'U':
                if (can_move(l, h->pos.x+1, h->pos.y-1)) {
                        do_move(h, 1, -1);
                        return !is_door(l->d, h->pos.x, h->pos.y);
                }
                return 0;
        case 'b':
                if (can_move(l, h->pos.x-1, h->pos.y+1))
                        do_move(h, -1, 1);
                return 0;
        case 'B':
                if (can_move(l, h->pos.x-1, h->pos.y+1)) {
                        do_move(h, -1, 1);
                        return !is_door(l->d, h->pos.x, h->pos.y);
                }
                return 0;
        case 'n':
                if (can_move(l, h->pos.x+1, h->pos.y+1))
                        do_move(h, 1, 1);
                return 0;
        case 'N':
                if (can_move(l, h->pos.x+1, h->pos.y+1)) {
                        do_move(h, 1, 1);
                        return !is_door(l->d, h->pos.x, h->pos.y);
                }
                return 0;
        default:
                return 0;
        }
}
