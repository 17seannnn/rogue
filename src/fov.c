/* fov.c - fov system */

#include "rogue.h"

static void fov_points(struct level *l, const struct room *r)
{
        if (get_room_by_coord(l->r, l->start.pos.x, l->start.pos.y) == r) {
                mvwaddch(gamew, l->start.pos.y, l->start.pos.x, l->start.symb);
                l->start.flags |= seen_flag;
        }
        if (get_room_by_coord(l->r, l->end.pos.x, l->end.pos.y) == r) {
                mvwaddch(gamew, l->end.pos.y, l->end.pos.x, l->end.symb);
                l->end.flags |= seen_flag;
        }
}

static void fov_loot(struct loot_list *l, struct coord tl, struct coord br)
{
        for ( ; l; l = l->next) {
                if (l->pos.x >= tl.x && l->pos.x <= br.x &&
                    l->pos.y >= tl.y && l->pos.y <= br.y) {
                        mvwaddch(gamew, l->pos.y, l->pos.x, loot_symb);
                        l->flags |= seen_flag;
                }
        }
}

static void fov_beasts(struct beast *b, struct coord tl, struct coord br)
{
        for ( ; b; b = b->next) {
                if (b->c.pos.x >= tl.x && b->c.pos.x <= br.x &&
                    b->c.pos.y >= tl.y && b->c.pos.y <= br.y) {
                        mvwaddch(gamew, b->c.pos.y, b->c.pos.x, b->c.symb);
                        b->c.flags |= seen_flag;
                }
        }
}

static void fov_room(struct level *l, const struct creature *h)
{
        struct room *r = get_room_by_coord(l->r, h->pos.x, h->pos.y);
        if (!r)
                return;
        show_room(r, l->d);
        fov_points(l, r);
        fov_loot(l->l, r->tl, r->br);
        fov_beasts(l->b, r->tl, r->br);
}

static void fov_path(struct level *l, const struct creature *h)
{
        int x, y;
        struct coord tl = h->pos, br = tl;
        struct path *p;
        struct room *r;
        tl.x -= 1;
        tl.y -= 1;
        br.x += 1;
        br.y += 1;
        for (x = tl.x; x <= br.x; x++) {
                for (y = tl.y; y <= br.y; y++) {
                        p = get_path_by_coord(l->p, x, y);
                        r = get_room_by_coord(l->r, x, y);
                        if (p) {
                                mvwaddch(gamew, y, x, path_symb);
                                p->flags |= seen_flag;
                        } else if (r) {
                                if (is_door(l->d, x, y))
                                        mvwaddch(gamew, y, x, door_symb);
                                else
                                        mvwaddch(gamew, y, x, wall_symb);
                                add_seen_wall(r, x, y);
                        }
                }
        }
        fov_loot(l->l, tl, br);
        fov_beasts(l->b, tl, br);
}

void handle_fov(struct level *l, const struct creature *h, int refresh)
{
/*
        const struct beast *b;
*/
        fov_path(l, h);
        fov_room(l, h);
        if (refresh) {
                show_creature(h);
                wrefresh(gamew);
        }
/*
        show_rooms(l->r, l->d);
        show_path(l->p);
        show_point(l->start);
        show_point(l->end);
        show_loot(l->l);
        show_creature(h);
        for (b = l->b; b; b = b->next)
                show_creature(&b->c);
*/
}
