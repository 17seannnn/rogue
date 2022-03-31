/* cmd.c - hunter`s cmd control */

#include "rogue.h"

static int get_char_side(int c)
{
        switch (c) {
        case 'h': case 'H': return side_west;
        case 'j': case 'J': return side_south;
        case 'k': case 'K': return side_north;
        case 'l': case 'L': return side_east;
        case 'y': case 'Y': return side_northwest;
        case 'u': case 'U': return side_northeast;
        case 'b': case 'B': return side_southwest;
        case 'n': case 'N': return side_southeast;
        default:            return -1;
        }
}

static int is_uppercase(int c)
{
        if (c >= 'A' && c <= 'Z')
                return 1;
        return 0;
}

static void try_move(struct level *l, struct creature *h, int c, int *flags)
{
        int side, x, y, res;
        side = get_char_side(c);
        get_side_diff(side, &x, &y);
        x += h->pos.x;
        y += h->pos.y;
        try_loot(l, h, side);
        res = move_creature(l, h, h, side);
        if (!res)
                try_attack_beast(h, l->b, side);
        if (is_uppercase(c)) {
                if (!res || is_door(l->d, x, y)) {
                        if (*flags & again)
                                *flags ^= again;
                } else {
                        *flags |= again;
                }
        }
}

void do_cmd(int c, struct creature *h, struct level *l, int *flags)
{
        int res;
        switch (c) {
        case 'h': case 'j': case 'k': case 'l':
        case 'y': case 'u': case 'b': case 'n':
        case 'H': case 'J': case 'K': case 'L':
        case 'Y': case 'U': case 'B': case 'N':
                try_move(l, h, c, flags);
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
