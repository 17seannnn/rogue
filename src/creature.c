/* creature.c - control creatures */

#include "rogue.h"

void show_creature(const struct creature *c)
{
        mvwaddch(gamew, c->pos.y, c->pos.x, c->symb);
        wrefresh(gamew);
}

/* TODO move other modules */
static int is_one_room(const struct room *r, struct coord pos1,
                                             struct coord pos2)
{
        if (get_room_by_coord(r, pos1.x, pos1.y) ==
            get_room_by_coord(r, pos2.x, pos2.y))
                return 1;
        return 0;
}

/*
 * If creatures are in one room or one of them is in door and
 *   first`s fov may him see second then return 1 else 0
 */
static int can_see(const struct level *l, const struct creature *c1,
                                          const struct creature *c2)
{
        if ((is_one_room(l->r, c1->pos, c2->pos) ||
             is_door(l->d, c1->pos.x, c1->pos.y) ||
             is_door(l->d, c2->pos.x, c2->pos.y)) &&
             c2->pos.x >= c1->pos.x - c1->fov     &&
             c2->pos.x <= c1->pos.x + c1->fov     &&
             c2->pos.y >= c1->pos.y - c1->fov     &&
             c2->pos.y <= c1->pos.y + c1->fov)
                return 1;
        return 0;
}

/* Return the direction to 2nd creature relative to the 1st one and -1 on fail */
int search_creature(const struct level *l, const struct creature *c1,
                                           const struct creature *c2)
{
        if (can_see(l, c1, c2)) {
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
