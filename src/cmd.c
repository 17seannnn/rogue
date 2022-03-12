/* cmd.c - hunter`s cmd control */

#include "rogue.h"

static int try_attack_beast(const struct creature *h, struct beast *b,
                             int side)
{
        int x, y;
        get_side_diff(side, &x, &y);
        x += h->pos.x;
        y += h->pos.y;
        for ( ; b; b = b->next)
                if (b->c.pos.x == x && b->c.pos.y == y)
                        return attack(h, &b->c);
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
                if (!res)
                        try_attack_beast(h, l->b, side_west);
                if (!res || is_door(l->d, h->pos.x, h->pos.y))
                        return 0;
                return 1;
        case 'j':
                res = move_creature(l, h, h, side_south);
                if (!res)
                        try_attack_beast(h, l->b, side_south);
                return 0;
        case 'J':
                res = move_creature(l, h, h, side_south);
                if (!res)
                        try_attack_beast(h, l->b, side_south);
                if (!res || is_door(l->d, h->pos.x, h->pos.y))
                        return 0;
                return 1;
        case 'k':
                res = move_creature(l, h, h, side_north);
                if (!res)
                        try_attack_beast(h, l->b, side_north);
                return 0;
        case 'K':
                res = move_creature(l, h, h, side_north);
                if (!res)
                        try_attack_beast(h, l->b, side_north);
                if (!res || is_door(l->d, h->pos.x, h->pos.y))
                        return 0;
                return 1;
        case 'l':
                res = move_creature(l, h, h, side_east);
                if (!res)
                        try_attack_beast(h, l->b, side_east);
                return 0;
        case 'L':
                res = move_creature(l, h, h, side_east);
                if (!res)
                        try_attack_beast(h, l->b, side_east);
                if (!res || is_door(l->d, h->pos.x, h->pos.y))
                        return 0;
                return 1;
        case 'y':
                res = move_creature(l, h, h, side_northwest);
                if (!res)
                        try_attack_beast(h, l->b, side_northwest);
                return 0;
        case 'Y':
                res = move_creature(l, h, h, side_northwest);
                if (!res)
                        try_attack_beast(h, l->b, side_northwest);
                if (!res || is_door(l->d, h->pos.x, h->pos.y))
                        return 0;
                return 1;
        case 'u':
                res = move_creature(l, h, h, side_northeast);
                if (!res)
                        try_attack_beast(h, l->b, side_northeast);
                return 0;
        case 'U':
                res = move_creature(l, h, h, side_northeast);
                if (!res)
                        try_attack_beast(h, l->b, side_northeast);
                if (!res || is_door(l->d, h->pos.x, h->pos.y))
                        return 0;
                return 1;
        case 'b':
                res = move_creature(l, h, h, side_southwest);
                if (!res)
                        try_attack_beast(h, l->b, side_southwest);
                return 0;
        case 'B':
                res = move_creature(l, h, h, side_southwest);
                if (!res)
                        try_attack_beast(h, l->b, side_southwest);
                if (!res || is_door(l->d, h->pos.x, h->pos.y))
                        return 0;
                return 1;
        case 'n':
                res = move_creature(l, h, h, side_southeast);
                if (!res)
                        try_attack_beast(h, l->b, side_southeast);
                return 0;
        case 'N':
                res = move_creature(l, h, h, side_southeast);
                if (!res)
                        try_attack_beast(h, l->b, side_southeast);
                if (!res || is_door(l->d, h->pos.x, h->pos.y))
                        return 0;
                return 1;
        case 'i':
                show_inv(h->inv);
                /* redraw screen */
                return 0;
        default:
                return 0;
        }
}
