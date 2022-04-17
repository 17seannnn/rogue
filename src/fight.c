/* fight.c - fight system */

#include <stdlib.h>

#include "rogue.h"

static const char *msg_hit[] = {
        " hit ",
        " have injured ",
        " swing and hit "
};

static const char msg_kill[] = " killed ";

int attack(struct creature *a, struct creature *d)
{
        int dmg;
        dmg = a->weapon ? a->weapon->l->val + a->dmg : a->dmg;
        d->hp -= dmg;
        add_msg(a->cast == cast_hunter ? "You" : "The beast");
        if (d->hp <= 0) {
                if (a->cast == cast_hunter)
                        a->blood += d->blood;
                append_msg(msg_kill);
        } else {
                append_msg(msg_hit[rand() % 3]);
        }
        append_msg(d->cast == cast_hunter ? "you." : "the beast.");
        return 1;
}
