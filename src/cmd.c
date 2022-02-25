/* cmd.c - hunter`s cmd control */

#include "rogue.h"

static void do_move(struct hunter *h, int dx, int dy)
{
        h->pos.x += dx;
        h->pos.y += dy;
}

void do_cmd(int c, struct hunter *h, struct level *l)
{
        switch (c) {
        case 'h':
                if (can_move(l, h->pos.x-1, h->pos.y))
                        do_move(h, -1, 0);
                break;
        case 'j':
                if (can_move(l, h->pos.x, h->pos.y+1))
                        do_move(h, 0, 1);
                break;
        case 'k':
                if (can_move(l, h->pos.x, h->pos.y-1))
                        do_move(h, 0, -1);
                break;
        case 'l':
                if (can_move(l, h->pos.x+1, h->pos.y))
                        do_move(h, 1, 0);
                break;
        case 'y':
                if (can_move(l, h->pos.x-1, h->pos.y-1))
                        do_move(h, -1, -1);
                break;
        case 'u':
                if (can_move(l, h->pos.x+1, h->pos.y-1))
                        do_move(h, 1, -1);
                break;
        case 'b':
                if (can_move(l, h->pos.x-1, h->pos.y+1))
                        do_move(h, -1, 1);
                break;
        case 'n':
                if (can_move(l, h->pos.x+1, h->pos.y+1))
                        do_move(h, 1, 1);
                break;
        default:
                break;
        }
}
