/* cmd.c - hunter`s cmd control */

#include "rogue.h"

static void move_hunter(int c, struct hunter *h, struct level *l)
{
        switch (c) {
        case 'h':
                if (can_move(l, h->pos.x-1, h->pos.y))
                        h->pos.x -= 1;
                break;
        case 'j':
                if (can_move(l, h->pos.x, h->pos.y+1))
                        h->pos.y += 1;
                break;
        case 'k':
                if (can_move(l, h->pos.x, h->pos.y-1))
                        h->pos.y -= 1;
                break;
        case 'l':
                if (can_move(l, h->pos.x+1, h->pos.y))
                        h->pos.x += 1;
                break;
        case 'y':
                if (can_move(l, h->pos.x-1, h->pos.y-1)) {
                        h->pos.x -= 1;
                        h->pos.y -= 1;
                }
                break;
        case 'u':
                if (can_move(l, h->pos.x+1, h->pos.y-1)) {
                        h->pos.x += 1;
                        h->pos.y -= 1;
                }
                break;
        case 'b':
                if (can_move(l, h->pos.x-1, h->pos.y+1)) {
                        h->pos.x -= 1;
                        h->pos.y += 1;
                }
                break;
        case 'n':
                if (can_move(l, h->pos.x+1, h->pos.y+1)) {
                        h->pos.x += 1;
                        h->pos.y += 1;
                }
                break;
        default:
                break;
        }
}

void do_cmd(int c, struct hunter *h, struct level *l)
{
                switch (c) {
                case 'h': case 'j': case 'k': case 'l':
                case 'y': case 'u': case 'b': case 'n':
                        move_hunter(c, h, l);
                        break;
                default:
                        break;
                }
}
