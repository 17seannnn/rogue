/* fight.c - fight system */

#include <stdlib.h>

#include "rogue.h"

static const char *msg_hit[] = {
        " hit ",
        " have injured ",
        " swing and hit "
};

int attack(const struct creature *a, struct creature *d)
{
        d->hp -= a->dmg;
        if (a->cast == cast_hunter) {
                add_msg("You");
                append_msg(msg_hit[rand() % 3]);
                append_msg("the beast.");
        } else {
                add_msg("The beast");
                append_msg(msg_hit[rand() % 3]);
                append_msg("you.");
        }
        return 1;
}
