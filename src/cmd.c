/* cmd.c - hunter`s cmd control */

#include "rogue.h"

int do_cmd(int c, struct creature *h, struct level *l)
{
        int res;
        switch (c) {
        case 'h':
                move_creature(l, h, h, side_west);
                return 0;
        case 'H':
                res = move_creature(l, h, h, side_west);
                return res ? !is_door(l->d, h->pos.x, h->pos.y) : 0;
        case 'j':
                move_creature(l, h, h, side_south);
                return 0;
        case 'J':
                res = move_creature(l, h, h, side_south);
                return res ? !is_door(l->d, h->pos.x, h->pos.y) : 0;
        case 'k':
                move_creature(l, h, h, side_north);
                return 0;
        case 'K':
                res = move_creature(l, h, h, side_north);
                return res ? !is_door(l->d, h->pos.x, h->pos.y) : 0;
        case 'l':
                move_creature(l, h, h, side_east);
                return 0;
        case 'L':
                res = move_creature(l, h, h, side_east);
                return res ? !is_door(l->d, h->pos.x, h->pos.y) : 0;
        case 'y':
                move_creature(l, h, h, side_northwest);
                return 0;
        case 'Y':
                res = move_creature(l, h, h, side_northwest);
                return res ? !is_door(l->d, h->pos.x, h->pos.y) : 0;
        case 'u':
                move_creature(l, h, h, side_northeast);
                return 0;
        case 'U':
                res = move_creature(l, h, h, side_northeast);
                return res ? !is_door(l->d, h->pos.x, h->pos.y) : 0;
        case 'b':
                move_creature(l, h, h, side_southwest);
                return 0;
        case 'B':
                res = move_creature(l, h, h, side_southwest);
                return res ? !is_door(l->d, h->pos.x, h->pos.y) : 0;
        case 'n':
                move_creature(l, h, h, side_southeast);
                return 0;
        case 'N':
                res = move_creature(l, h, h, side_southeast);
                return res ? !is_door(l->d, h->pos.x, h->pos.y) : 0;
        default:
                return 0;
        }
}
