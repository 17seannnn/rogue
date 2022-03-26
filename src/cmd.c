/* cmd.c - hunter`s cmd control */

#include "rogue.h"

void do_cmd(int c, struct creature *h, struct level *l, int *flags)
{
        int res;
        switch (c) {
        case 'h':
                try_loot(l, h, side_west);
                res = move_creature(l, h, h, side_west);
                if (!res)
                        try_attack_beast(h, l->b, side_west);
                break;
        case 'H':
                try_loot(l, h, side_west);
                res = move_creature(l, h, h, side_west);
                if (!res)
                        try_attack_beast(h, l->b, side_west);
                if (!res || is_door(l->d, h->pos.x, h->pos.y)) {
                        if (*flags & again)
                                *flags ^= again;
                } else {
                        *flags |= again;
                }
                break;
        case 'j':
                try_loot(l, h, side_south);
                res = move_creature(l, h, h, side_south);
                if (!res)
                        try_attack_beast(h, l->b, side_south);
                break;
        case 'J':
                try_loot(l, h, side_south);
                res = move_creature(l, h, h, side_south);
                if (!res)
                        try_attack_beast(h, l->b, side_south);
                if (!res || is_door(l->d, h->pos.x, h->pos.y)) {
                        if (*flags & again)
                                *flags ^= again;
                } else {
                        *flags |= again;
                }
                break;
        case 'k':
                try_loot(l, h, side_north);
                res = move_creature(l, h, h, side_north);
                if (!res)
                        try_attack_beast(h, l->b, side_north);
                break;
        case 'K':
                try_loot(l, h, side_north);
                res = move_creature(l, h, h, side_north);
                if (!res)
                        try_attack_beast(h, l->b, side_north);
                if (!res || is_door(l->d, h->pos.x, h->pos.y)) {
                        if (*flags & again)
                                *flags ^= again;
                } else {
                        *flags |= again;
                }
                break;
        case 'l':
                try_loot(l, h, side_east);
                res = move_creature(l, h, h, side_east);
                if (!res)
                        try_attack_beast(h, l->b, side_east);
                break;
        case 'L':
                try_loot(l, h, side_east);
                res = move_creature(l, h, h, side_east);
                if (!res)
                        try_attack_beast(h, l->b, side_east);
                if (!res || is_door(l->d, h->pos.x, h->pos.y)) {
                        if (*flags & again)
                                *flags ^= again;
                } else {
                        *flags |= again;
                }
                break;
        case 'y':
                try_loot(l, h, side_northwest);
                res = move_creature(l, h, h, side_northwest);
                if (!res)
                        try_attack_beast(h, l->b, side_northwest);
                break;
        case 'Y':
                try_loot(l, h, side_northwest);
                res = move_creature(l, h, h, side_northwest);
                if (!res)
                        try_attack_beast(h, l->b, side_northwest);
                if (!res || is_door(l->d, h->pos.x, h->pos.y)) {
                        if (*flags & again)
                                *flags ^= again;
                } else {
                        *flags |= again;
                }
                break;
        case 'u':
                try_loot(l, h, side_northeast);
                res = move_creature(l, h, h, side_northeast);
                if (!res)
                        try_attack_beast(h, l->b, side_northeast);
                break;
        case 'U':
                try_loot(l, h, side_northeast);
                res = move_creature(l, h, h, side_northeast);
                if (!res)
                        try_attack_beast(h, l->b, side_northeast);
                if (!res || is_door(l->d, h->pos.x, h->pos.y)) {
                        if (*flags & again)
                                *flags ^= again;
                } else {
                        *flags |= again;
                }
                break;
        case 'b':
                try_loot(l, h, side_southwest);
                res = move_creature(l, h, h, side_southwest);
                if (!res)
                        try_attack_beast(h, l->b, side_southwest);
                break;
        case 'B':
                try_loot(l, h, side_southwest);
                res = move_creature(l, h, h, side_southwest);
                if (!res)
                        try_attack_beast(h, l->b, side_southwest);
                if (!res || is_door(l->d, h->pos.x, h->pos.y)) {
                        if (*flags & again)
                                *flags ^= again;
                } else {
                        *flags |= again;
                }
                break;
        case 'n':
                try_loot(l, h, side_southeast);
                res = move_creature(l, h, h, side_southeast);
                if (!res)
                        try_attack_beast(h, l->b, side_southeast);
                break;
        case 'N':
                try_loot(l, h, side_southeast);
                res = move_creature(l, h, h, side_southeast);
                if (!res)
                        try_attack_beast(h, l->b, side_southeast);
                if (!res || is_door(l->d, h->pos.x, h->pos.y)) {
                        if (*flags & again)
                                *flags ^= again;
                } else {
                        *flags |= again;
                }
                break;
        case 'i':
        case '*':
                show_inv(h);
                break;
        case 'd':
                drop(&l->l, h);
                break;
        case 'w':
                wield(h);
                break;
        case 'W':
                wear(h);
                break;
        case 'q':
                quaff(h);
                break;
        default:
                break;
        }
}
