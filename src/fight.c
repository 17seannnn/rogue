/* fight.c - fight system */

#include <stdlib.h>
#include <string.h>

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
	char buf[bufsize];
        dmg = a->weapon ? a->weapon->l->val + a->dmg : a->dmg;
        d->hp -= dmg;
        add_msg(a->cast == cast_hunter ? "You" : a->name);
        if (d->hp <= 0) {
		if (a->cast == cast_hunter)
			add_blood(a, d->blood);
                append_msg(msg_kill);
        } else {
                append_msg(msg_hit[rand() % 3]);
        }
	if (d->cast == cast_hunter) {
		append_msg("you.");
	} else {
		strncpy(buf, d->name, bufsize);
		if (buf[0] >= 'A' && buf[0] <= 'Z')
			buf[0] += 'a' - 'A';
		append_msg(buf);
		append_msg(".");
	}
        return 1;
}
