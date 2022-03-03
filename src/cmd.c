/* cmd.c - hunter`s cmd control */

#include "rogue.h"

static int try_attack_beast(const struct creature *h, struct beast *b,
                             int side)
{
        int x = h->pos.x, y = h->pos.y;
        switch (side) {
        case side_northwest:
                x--;
                y--;
                break;
        case side_north:
                y--;
                break;
        case side_northeast:
                x++;
                y--;
                break;
        case side_east:
                x++;
                break;
        case side_southeast:
                x++;
                y++;
                break;
        case side_south:
                y++;
                break;
        case side_southwest:
                x--;
                y++;
                break;
        case side_west:
                x--;
                break;
        }
        for ( ; b; b = b->next) {
                if (b->c.pos.x == x && b->c.pos.y == y) {
                        return attack(h, &b->c);
                }
        }
        return 0;
}

int do_cmd(int c, struct creature *h, struct level *l)
{
        int res;
        switch (c) {
        case 'h':
                res = move_creature(l, h, h, side_west);
                if (!res)
                        try_attack_beast(h, l->b, side_west);
                return 0;
        case 'H':
                res = move_creature(l, h, h, side_west);
                return res ? !is_door(l->d, h->pos.x, h->pos.y) : 0;
        case 'j':
                res = move_creature(l, h, h, side_south);
                if (!res)
                        try_attack_beast(h, l->b, side_south);
                return 0;
        case 'J':
                res = move_creature(l, h, h, side_south);
                return res ? !is_door(l->d, h->pos.x, h->pos.y) : 0;
        case 'k':
                res = move_creature(l, h, h, side_north);
                if (!res)
                        try_attack_beast(h, l->b, side_north);
                return 0;
        case 'K':
                res = move_creature(l, h, h, side_north);
                return res ? !is_door(l->d, h->pos.x, h->pos.y) : 0;
        case 'l':
                res = move_creature(l, h, h, side_east);
                if (!res)
                        try_attack_beast(h, l->b, side_east);
                return 0;
        case 'L':
                res = move_creature(l, h, h, side_east);
                return res ? !is_door(l->d, h->pos.x, h->pos.y) : 0;
        case 'y':
                res = move_creature(l, h, h, side_northwest);
                if (!res)
                        try_attack_beast(h, l->b, side_northwest);
                return 0;
        case 'Y':
                res = move_creature(l, h, h, side_northwest);
                return res ? !is_door(l->d, h->pos.x, h->pos.y) : 0;
        case 'u':
                res = move_creature(l, h, h, side_northeast);
                if (!res)
                        try_attack_beast(h, l->b, side_northeast);
                return 0;
        case 'U':
                res = move_creature(l, h, h, side_northeast);
                return res ? !is_door(l->d, h->pos.x, h->pos.y) : 0;
        case 'b':
                res = move_creature(l, h, h, side_southwest);
                if (!res)
                        try_attack_beast(h, l->b, side_southwest);
                return 0;
        case 'B':
                res = move_creature(l, h, h, side_southwest);
                return res ? !is_door(l->d, h->pos.x, h->pos.y) : 0;
        case 'n':
                res = move_creature(l, h, h, side_southeast);
                if (!res)
                        try_attack_beast(h, l->b, side_southeast);
                return 0;
        case 'N':
                res = move_creature(l, h, h, side_southeast);
                return res ? !is_door(l->d, h->pos.x, h->pos.y) : 0;
        default:
                return 0;
        }
}
