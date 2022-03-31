/* cmd.c - hunter`s cmd control */

#include "rogue.h"

void do_cmd(int c, struct creature *h, struct level *l, int *flags)
{
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
