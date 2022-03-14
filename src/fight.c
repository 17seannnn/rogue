/* fight.c - fight system */

#include <stdlib.h>

#include "rogue.h"

static const char *msg_hit[] = {
        " hit ",
        " have injured ",
        " swing and hit "
};

static const char msg_kill[] = " killed ";

int attack(const struct creature *a, struct creature *d)
{
        int dmg;
        dmg = a->weapon ? a->weapon->dmg + a->dmg : a->dmg;
        d->hp -= dmg;
        add_msg(a->cast == cast_hunter ? "You" : "The beast");
        append_msg(d->hp > 0 ? msg_hit[rand() % 3] : msg_kill);
        append_msg(d->cast == cast_hunter ? "you." : "the beast.");
        return 1;
}
