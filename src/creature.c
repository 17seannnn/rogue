/* creature.c - control creatures */

#include "rogue.h"

void show_creature(const struct creature *c)
{
        mvwaddch(gamew, c->pos.y, c->pos.x, c->symb);
}

/*
 * Return 1:
 * if first creature is beast and saw hunter before
 * if first and second are in one room and first can see with his fov second
 * if first or second stands at the door and first can see with his fov second
 */
static int can_see(const struct level *l, const struct creature *c1,
                                          const struct creature *c2)
{
        if ((c1->cast == cast_beast && c1->flags & saw_hunter_flag) ||
            ((is_one_room(l->r, c1->pos, c2->pos) ||
              is_door(l->d, c1->pos.x, c1->pos.y) ||
              is_door(l->d, c2->pos.x, c2->pos.y)) &&
             c2->pos.x >= c1->pos.x - c1->fov      &&
             c2->pos.x <= c1->pos.x + c1->fov      &&
             c2->pos.y >= c1->pos.y - c1->fov      &&
             c2->pos.y <= c1->pos.y + c1->fov))
                return 1;
        return 0;
}

/*
 * Return the direction to 2nd creature relative to
 *   the 1st one and -1 on fail
 */
int search_creature(const struct level *l, struct creature *c1,
                                     const struct creature *c2)
{
        if (can_see(l, c1, c2)) {
                if (c1->cast == cast_beast)
                        c1->flags |= saw_hunter_flag;
                if (c2->pos.x < c1->pos.x && c2->pos.y < c1->pos.y)
                        return side_northwest;
                if (c2->pos.x == c1->pos.x && c2->pos.y < c1->pos.y)
                        return side_north;
                if (c2->pos.x > c1->pos.x && c2->pos.y < c1->pos.y)
                        return side_northeast;
                if (c2->pos.x > c1->pos.x && c2->pos.y == c1->pos.y)
                        return side_east;
                if (c2->pos.x > c1->pos.x && c2->pos.y > c1->pos.y)
                        return side_southeast;
                if (c2->pos.x == c1->pos.x && c2->pos.y > c1->pos.y)
                        return side_south;
                if (c2->pos.x < c1->pos.x && c2->pos.y > c1->pos.y)
                        return side_southwest;
                if (c2->pos.x < c1->pos.x && c2->pos.y == c1->pos.y)
                        return side_west;
        }
        return -1;
}

int move_creature(const struct level *l, const struct creature *h,
                  struct creature *c, int side)
{
        int x, y;
        get_side_diff(side, &x, &y);
        x += c->pos.x;
        y += c->pos.y;
        if (can_move(l, h, x, y)) {
                c->pos.x = x;
                c->pos.y = y;
                return 1;
        }
        return 0;
}

int add_health(struct creature *c, int val)
{
	if (c->hp == c->max_hp)
		return 0;
	c->hp += val;
	if (c->hp > c->max_hp)
		c->hp = c->max_hp;
	return 1;
}

void add_blood(struct level *l, struct creature *c, int val)
{
	int remainder;
	struct loot bl;
	c->blood += val;
	if (c->blood > max_blood) {
		remainder = c->blood - max_blood;
		c->blood = max_blood;
		bl = blood_list[blood_debug];
		bl.val = remainder;
		add_loot(&l->l, &bl, c->pos.x, c->pos.y, 0);
	}
}

int get_max_exp(int level)
{
	return (int)power(2, level);
}

void add_exp(struct creature *c, int val)
{
	int l = c->level, e = c->exp + val;
	if (e >= 0 && e < get_max_exp(l))
		return;
	if (e > 0) {
		while (l < max_creature_level &&
		       e - get_max_exp(l) >= 0) {
			e -= get_max_exp(l);
			l++;
		}
		if (l == max_creature_level && e > get_max_exp(l))
			e = get_max_exp(l);
	} else {
		while (l > 0 && e < 0) {
			e += get_max_exp(l-1);
			l--;
		}
		if (l == 0 && e < 0)
			e = 0;
	}
	c->level = l;
	c->exp = e;
}
