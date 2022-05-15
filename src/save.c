#include <stdio.h>
#include <string.h>

#include "rogue.h"

static char save_fn[bufsize];

void set_save_fn(const char *fn)
{
	strncpy(save_fn, fn, bufsize);
}

int has_save()
{
	FILE *f = fopen(save_fn, "rb");
	if (!f)
		return 0;
	fclose(f);
	return 1;
}

void save_game(const struct level *l, const struct creature *h)
{

}

void load_game(struct level *l, struct creature *h)
{

}
