/* fov.c - fov system */

#include "rogue.h"

static void fov_points(struct level *l, const struct room *r)
{
        if (get_room_by_coord(l->r, l->start.pos.x, l->start.pos.y) == r) {
                show_point(l->start);
                l->start.flags |= seen_flag;
        }
        if (get_room_by_coord(l->r, l->end.pos.x, l->end.pos.y) == r) {
                show_point(l->end);
                l->end.flags |= seen_flag;
        }
}

static void fov_loot(struct loot_list *l, struct coord tl, struct coord br)
{
        int x, y;
        struct loot_list *t;
        for (x = tl.x-1; x <= tl.x+1; x++) {
                for (y = tl.y-1; y <= tl.y+1; y++) {
                        for (t = l; t; t = t->next) {
                                if (t->pos.x >= tl.x && t->pos.x <= br.x &&
                                    t->pos.y >= tl.y && t->pos.y <= br.y) {
                                        mvwaddch(gamew, t->pos.y, t->pos.x,
                                                 loot_symb);
                                        t->flags |= seen_flag;
                                }
                        }
                }
        }
}

static void fov_beasts(struct beast *b, struct coord tl, struct coord br)
{
        int x, y;
        struct beast *t;
        for (x = tl.x-1; x <= tl.x+1; x++) {
                for (y = tl.y-1; y <= tl.y+1; y++) {
                        for (t = b; t; t = t->next) {
                                if (t->c.pos.x >= tl.x && t->c.pos.x <= br.x &&
                                    t->c.pos.y >= tl.y && t->c.pos.y <= br.y) {
                                        mvwaddch(gamew, t->c.pos.y, t->c.pos.x,
                                                 t->c.symb);
                                        t->c.flags |= seen_flag;
                                }
                        }
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
        show_creature(h);
}

void handle_fov(struct level *l, const struct creature *h)
{
/*
        const struct beast *b;
*/
        fov_room(l, h);
        fov_path(l, h);
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
