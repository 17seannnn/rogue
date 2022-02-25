/* cmd.c - hunter`s cmd control */

#include "rogue.h"

static void move_hunter(int c, struct hunter *h, struct level *l)
{
        switch (c) {
        case 'h':
        case KEY_LEFT:
                if (can_move(l, h->pos.x-1, h->pos.y))
                        h->pos.x -= 1;
                break;
        case 'j':
        case KEY_DOWN:
                if (can_move(l, h->pos.x, h->pos.y+1))
                        h->pos.y += 1;
                break;
        case 'k':
        case KEY_UP:
                if (can_move(l, h->pos.x, h->pos.y-1))
                        h->pos.y -= 1;
                break;
        case 'l':
        case KEY_RIGHT:
                if (can_move(l, h->pos.x+1, h->pos.y))
                        h->pos.x += 1;
                break;
        default:
                break;
        }
}

void do_cmd(int c, struct hunter *h, struct level *l)
{
                switch (c) {
                case 'h': case 'j': case 'k': case 'l':
                case KEY_LEFT: case KEY_DOWN: case KEY_UP: case KEY_RIGHT:
                        move_hunter(c, h, l);
                        break;
                default:
                        break;
                }
}
