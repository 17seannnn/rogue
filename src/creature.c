/* creature.c - control creatures */

#include "rogue.h"

void show_creature(const struct creature *c)
{
        mvwaddch(gamew, c->pos.y, c->pos.x, c->symb);
        wrefresh(gamew);
}

/* Return the direction to 2nd creature relative to the 1st one and -1 on fail */
int search_creature(const struct creature *c1, const struct creature *c2)
{
        if (c2->pos.x >= c1->pos.x - c1->fov && c2->pos.x <= c1->pos.x + c1->fov &&
            c2->pos.y >= c1->pos.y - c1->fov && c2->pos.y <= c1->pos.y + c1->fov) {
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
