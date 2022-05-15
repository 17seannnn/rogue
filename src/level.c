/* level.c - level control*/

#include <stdlib.h>

#include "rogue.h"

const struct level_type level_list[] = {
        {
	  0,
	  4,
	  1, 3,
	  { 50, 30, 30 },
          { 50, 30, 30, 5 },
	  '<', '>', '-', '|', '+', '.', '#', ':',
	  { -1, COLOR_MAGENTA+8 }, { -1, COLOR_MAGENTA+8 }, { COLOR_BLUE, -1 }, { COLOR_BLUE, -1 },
	  { -1, -1 }, { -1, -1 }, { -1, -1 }, { -1, COLOR_BLUE },
	  { COLOR_MAGENTA+8, -1 }, { COLOR_MAGENTA+8, -1 }, { COLOR_CYAN+8, -1 }, { COLOR_BLUE, -1 }
	}
};

int start_symb, end_symb, hor_wall_symb, ver_wall_symb,
    door_symb, ground_symb, path_symb, loot_symb;

int fov_start_symb, fov_end_symb, fov_path_symb, fov_loot_symb;

int is_linked_coord(struct linked_coord *lc, int x, int y)
{
        for ( ; lc; lc = lc->next)
                if (lc->pos.x == x && lc->pos.y == y)
                        return 1;
        return 0;
}

void add_linked_coord(struct linked_coord **lc, int x, int y)
{
        struct linked_coord *t;
        if (is_linked_coord(*lc, x, y))
                return;
        t = malloc(sizeof(*t));
        t->pos.x = x;
        t->pos.y = y;
        t->next = *lc;
        *lc = t;
}

void free_linked_coord(struct linked_coord *lc)
{
        struct linked_coord *t;
        while (lc) {
                t = lc;
                lc = lc->next;
                free(t);
        }
}

int is_ok(int x, int y)
{
        return x >= 0 && x < gamew_col && y >= 0 && y < gamew_row;
}

int is_empty(const struct level *l, int x, int y)
{
        return is_ok(x, y) && !is_room(l->r, x, y) &&
               !is_path(l->p, x, y) && !is_door(l->d, x, y);
}

int is_stop(const struct level *l, int x, int y)
{
        const struct door *d;
        const struct loot_list *ll;
        if ((l->start.pos.x == x && l->start.pos.y == y) ||
            (l->end.pos.x   == x && l->end.pos.y   == y))
                return 1;
        for (d = l->d; d; d = d->next)
                if (d->pos.x == x && d->pos.y == y)
                        return 1;
        for (ll = l->l; ll; ll = ll->next)
                if (ll->pos.x == x && ll->pos.y == y)
                        return 1;
        return 0;
}

int can_move(const struct level *l, const struct creature *h, int x, int y)
{
        return !is_hunter(h, x, y) && !is_beast(l->b, x, y) &&
               ((is_room(l->r, x, y) && (!is_wall(l->r, x, y) ||
                is_door(l->d, x, y))) || is_path(l->p, x, y));
}

int get_char_side(int c)
{
        switch (c) {
        case 'h': case 'H': return side_west;
        case 'j': case 'J': return side_south;
        case 'k': case 'K': return side_north;
        case 'l': case 'L': return side_east;
        case 'y': case 'Y': return side_northwest;
        case 'u': case 'U': return side_northeast;
        case 'b': case 'B': return side_southwest;
        case 'n': case 'N': return side_southeast;
        default:            return -1;
        }
}

void get_side_diff(int side, int *dx, int *dy)
{
        switch (side) {
        case side_northwest:
                *dx = -1;
                *dy = -1;
                break;
        case side_north:
                *dx = 0;
                *dy = -1;
                break;
        case side_northeast:
                *dx = 1;
                *dy = -1;
                break;
        case side_east:
                *dx = 1;
                *dy = 0;
                break;
        case side_southeast:
                *dx = 1;
                *dy = 1;
                break;
        case side_south:
                *dx = 0;
                *dy = 1;
                break;
        case side_southwest:
                *dx = -1;
                *dy = 1;
                break;
        case side_west:
                *dx = -1;
                *dy = 0;
                break;
        default:
                *dx = 0;
                *dy = 0;
        }
}

int can_side(const struct level *l, int side, int x, int y)
{
        int dx, dy;
        get_side_diff(side, &dx, &dy);
        x += dx;
        y += dy;
        return !is_beast(l->b, x, y) && ((is_room(l->r, x, y) &&
               (!is_wall(l->r, x, y) || is_door(l->d, x, y))) ||
                is_path(l->p, x, y));
}

/* Return side random side if can`t go throught first one */
int try_side(const struct level *l, int side, int x, int y)
{
        if (can_side(l, side, x, y)) {
                return side;
        } else {
                if (rand() % 2) {
                        if (can_side(l, side-1, x, y))
                                return side - 1;
                        else if (can_side(l, side+1, x, y))
                                return side + 1;
                } else {
                        if (can_side(l, side+1, x, y))
                                return side + 1;
                        else if (can_side(l, side-1, x, y))
                                return side - 1;
                }
        }
        return rand() % 8;
}

static void init_points(struct level *l)
{
        struct room *r;
        r = get_room_by_idx(l->r, 'A', 1);
        l->start.pos.x = r->tl.x + 1 + rand() % (room_len(r, 'h') - 2);
        l->start.pos.y = r->tl.y + 1 + rand() % (room_len(r, 'v') - 2);
        l->start.flags = 0;
        switch (l->depth) {
        case 1:
                r = get_room_by_idx(l->r, 'A', 2);
                break;
        case 2:
                r = get_room_by_idx(l->r, 'B', 2);
                break;
        case 3:
                r = get_room_by_idx(l->r, 'B', 4);
                break;
        case 4:
                r = get_room_by_idx(l->r, 'D', 4);
                break;
        }
        l->end.pos.x = r->tl.x + 1 + rand() % (room_len(r, 'h') - 2);
        l->end.pos.y = r->tl.y + 1 + rand() % (room_len(r, 'v') - 2);
        l->end.flags = 0;
}

static void init_leveltype(struct level *l, int lt)
{
	l->lt = &level_list[lt];

	set_pair(start_pair,    &l->lt->start_color);
	set_pair(end_pair,      &l->lt->end_color);
	set_pair(hor_wall_pair, &l->lt->hor_wall_color);
	set_pair(ver_wall_pair, &l->lt->ver_wall_color);
	set_pair(door_pair,     &l->lt->door_color);
	set_pair(ground_pair,   &l->lt->ground_color);
	set_pair(path_pair,     &l->lt->path_color);
	set_pair(loot_pair,     &l->lt->loot_color);

	set_pair(fov_start_pair, &l->lt->fov_start_color);
	set_pair(fov_end_pair,   &l->lt->fov_end_color);
	set_pair(fov_path_pair,  &l->lt->fov_path_color);
	set_pair(fov_loot_pair,  &l->lt->fov_loot_color);

	start_symb    = l->lt->start_symb    | COLOR_PAIR(start_pair);
	end_symb      = l->lt->end_symb      | COLOR_PAIR(end_pair);
	hor_wall_symb = l->lt->hor_wall_symb | COLOR_PAIR(hor_wall_pair);
	ver_wall_symb = l->lt->ver_wall_symb | COLOR_PAIR(ver_wall_pair);
	door_symb     = l->lt->door_symb     | COLOR_PAIR(door_pair);
	ground_symb   = l->lt->ground_symb   | COLOR_PAIR(ground_pair);
	path_symb     = l->lt->path_symb     | COLOR_PAIR(path_pair);
	loot_symb     = l->lt->loot_symb     | COLOR_PAIR(loot_pair);

	fov_start_symb = l->lt->start_symb | COLOR_PAIR(fov_start_pair);
	fov_end_symb   = l->lt->end_symb   | COLOR_PAIR(fov_end_pair);
	fov_path_symb  = l->lt->path_symb  | COLOR_PAIR(fov_path_pair);
	fov_loot_symb  = l->lt->loot_symb  | COLOR_PAIR(fov_loot_pair);
}

void init_level(struct level *l, struct creature *h)
{
	int hs = has_save();
	/*
	 * Save contains {
	 *   level count,
	 *   hunter except from his position
	 * }
	 */
	if (hs)
		load_game(l, h);
	else
		l->count = 0;
	init_leveltype(l, l->count/5);
        l->depth = init_room(l);
        init_path(l);
        init_points(l);
	init_hunter(l, h, hs);
        init_beast(l, h);
        init_loot(l, h);
}

void end_level(struct level *l)
{
        free_room(l->r);
        free_path(l->p);
        free_door(l->d);
        free_beast(l->b);
        free_loot(l->l);
        wclear(msgw);
        wclear(gamew);
        wclear(infow);
        wrefresh(msgw);
        wrefresh(gamew);
        wrefresh(infow);
}
