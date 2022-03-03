/* fight.c - fight system */

#include "rogue.h"

int attack(const struct creature *a, struct creature *d)
{
        d->hp -= a->dmg;
        return 1;
}
