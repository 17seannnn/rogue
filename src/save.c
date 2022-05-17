#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "rogue.h"

struct save_state {
	int level_count;
	int has_weapon;
	int has_armor;
	int inv_count;
};

static char save_fn[bufsize] = "";

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
	struct save_state st;
	struct loot_list *inv;
	FILE *f = fopen(save_fn, "wb");
	if (!f) {
		/*add_msg("Error while saving: %s", strerror(errno));*/
		return;
	}
	st.level_count = l->count;
	st.has_weapon  = h->weapon ? 1 : 0;
	st.has_armor   = h->armor  ? 1 : 0;
	st.inv_count   = count_loot(h->inv);
	fwrite(&st, sizeof(st), 1, f);
	fwrite(h, sizeof(*h), 1, f);
	if (st.has_weapon) {
		fwrite(&h->weapon->l, sizeof(struct loot), 1, f);
	}
	if (st.has_armor)
		fwrite(&h->armor->l, sizeof(struct loot), 1, f);
	for (inv = h->inv; inv; inv = inv->next)
		fwrite(&inv->l, sizeof(struct loot), 1, f);
	fclose(f);
}

int load_game(struct level *l, struct creature *h)
{
	int i;
	struct save_state st;
	struct loot w, a, t;
	struct loot_list *inv;
	FILE *f = fopen(save_fn, "rb");
	if (!f) {
		/*add_msg("Error while loading: %s", strerror(errno));*/
		return 0;
	}
	if (fread(&st, sizeof(st), 1, f) <= 0) {
		fclose(f);
		return 0;
	}
	fread(h, sizeof(*h), 1, f);
	if (st.has_weapon)
		fread(&w, sizeof(w), 1, f);
	else
		h->weapon = NULL;
	if (st.has_armor)
		fread(&a, sizeof(a), 1, f);
	else
		h->armor = NULL;
	for (i = 0, h->inv = NULL; i < st.inv_count; i++) {
		fread(&t, sizeof(t), 1, f);
		add_loot(&h->inv, &t, 0, 0, 0);
	}
	if (st.has_weapon || st.has_armor) {
		for (inv = h->inv; inv; inv = inv->next) {
			if (st.has_weapon &&
                            0 == memcmp(&inv->l, &w, sizeof(struct loot))) {
				h->weapon = inv;
				st.has_weapon = 0;
			} else
			if (st.has_armor &&
                            0 == memcmp(&inv->l, &a, sizeof(struct loot))) {
				h->armor = inv;
				st.has_armor = 0;
			}
		}
	}
	fclose(f);
	return 1;
}
