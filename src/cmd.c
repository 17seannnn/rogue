/* cmd.c - hunter`s cmd control */

#include "rogue.h"

int do_cmd(int c, struct creature *h, struct level *l)
{
        int res;
        switch (c) {
        case 'h':
                try_loot(l, h, side_west);
                res = move_creature(l, h, h, side_west);
                if (!res)
                        try_attack_beast(h, l->b, side_west);
                return 0;
        case 'H':
                try_loot(l, h, side_west);
                res = move_creature(l, h, h, side_west);
                if (!res)
                        try_attack_beast(h, l->b, side_west);
                if (!res || is_door(l->d, h->pos.x, h->pos.y))
                        return 0;
                return 1;
        case 'j':
                try_loot(l, h, side_south);
                res = move_creature(l, h, h, side_south);
                if (!res)
                        try_attack_beast(h, l->b, side_south);
                return 0;
        case 'J':
                try_loot(l, h, side_south);
                res = move_creature(l, h, h, side_south);
                if (!res)
                        try_attack_beast(h, l->b, side_south);
                if (!res || is_door(l->d, h->pos.x, h->pos.y))
                        return 0;
                return 1;
        case 'k':
                try_loot(l, h, side_north);
                res = move_creature(l, h, h, side_north);
                if (!res)
                        try_attack_beast(h, l->b, side_north);
                return 0;
        case 'K':
                try_loot(l, h, side_north);
                res = move_creature(l, h, h, side_north);
                if (!res)
                        try_attack_beast(h, l->b, side_north);
                if (!res || is_door(l->d, h->pos.x, h->pos.y))
                        return 0;
                return 1;
        case 'l':
                try_loot(l, h, side_east);
                res = move_creature(l, h, h, side_east);
                if (!res)
                        try_attack_beast(h, l->b, side_east);
                return 0;
        case 'L':
                try_loot(l, h, side_east);
                res = move_creature(l, h, h, side_east);
                if (!res)
                        try_attack_beast(h, l->b, side_east);
                if (!res || is_door(l->d, h->pos.x, h->pos.y))
                        return 0;
                return 1;
        case 'y':
                try_loot(l, h, side_northwest);
                res = move_creature(l, h, h, side_northwest);
                if (!res)
                        try_attack_beast(h, l->b, side_northwest);
                return 0;
        case 'Y':
                try_loot(l, h, side_northwest);
                res = move_creature(l, h, h, side_northwest);
                if (!res)
                        try_attack_beast(h, l->b, side_northwest);
                if (!res || is_door(l->d, h->pos.x, h->pos.y))
                        return 0;
                return 1;
        case 'u':
                try_loot(l, h, side_northeast);
                res = move_creature(l, h, h, side_northeast);
                if (!res)
                        try_attack_beast(h, l->b, side_northeast);
                return 0;
        case 'U':
                try_loot(l, h, side_northeast);
                res = move_creature(l, h, h, side_northeast);
                if (!res)
                        try_attack_beast(h, l->b, side_northeast);
                if (!res || is_door(l->d, h->pos.x, h->pos.y))
                        return 0;
                return 1;
        case 'b':
                try_loot(l, h, side_southwest);
                res = move_creature(l, h, h, side_southwest);
                if (!res)
                        try_attack_beast(h, l->b, side_southwest);
                return 0;
        case 'B':
                try_loot(l, h, side_southwest);
                res = move_creature(l, h, h, side_southwest);
                if (!res)
                        try_attack_beast(h, l->b, side_southwest);
                if (!res || is_door(l->d, h->pos.x, h->pos.y))
                        return 0;
                return 1;
        case 'n':
                try_loot(l, h, side_southeast);
                res = move_creature(l, h, h, side_southeast);
                if (!res)
                        try_attack_beast(h, l->b, side_southeast);
                return 0;
        case 'N':
                try_loot(l, h, side_southeast);
                res = move_creature(l, h, h, side_southeast);
                if (!res)
                        try_attack_beast(h, l->b, side_southeast);
                if (!res || is_door(l->d, h->pos.x, h->pos.y))
                        return 0;
                return 1;
        case 'i':
        case '*':
                show_inv(h);
                return 0;
        case 'd':
                drop(&l->l, h);
                return 0;
        case 'w':
                wield(h);
                return 0;
        case 'W':
                wear(h);
                return 0;
        case 'q':
                quaff(h);
                return 0;
        default:
                return 0;
        }
}
