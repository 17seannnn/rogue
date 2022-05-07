/* fight.c - fight system */

#include <stdlib.h>
#include <string.h>

#include "rogue.h"

enum {
	msg_hit_count  = 3,
	msg_miss_count = 2,
	miss_chance    = 5
};

static const char *msg_hit[] = {
        " hit ",
        " have injured ",
        " swing and hit "
};
static const char *msg_miss[] = {
	" miss ",
	" don't hit "
};
static const char msg_kill[] = " killed ";

int attack(struct level *l, struct creature *a, struct creature *d)
{
        int dmg;
	char buf[bufsize];
        add_msg(a->cast == cast_hunter ? "You" : a->name);
	if (rand() % 100 >= miss_chance) {
		dmg = a->weapon ? a->weapon->l.val + a->dmg : a->dmg;
		d->hp -= dmg;
		if (d->hp <= 0) {
			if (a->cast == cast_hunter) {
				add_blood(l, a, d->blood);
				add_exp(a, d->exp);
			}
			append_msg(msg_kill);
		} else {
			append_msg(msg_hit[rand() % msg_hit_count]);
		}
	} else {
		append_msg(msg_miss[rand() % msg_miss_count]);
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
