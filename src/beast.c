#include <stdlib.h>

#include "rogue.h"

static void add_beast(struct beast **b, int x, int y)
{
        struct beast *t;
        t = malloc(sizeof(*t));
        t->symb = 'B';
        t->pos.x = x;
        t->pos.y = y;
        t->hp = 10;
        t->dmg = 1;
        t->next = *b;
        t->fov = 5;
        *b = t;
}

int is_beast(const struct beast *b, int x, int y)
{
        for ( ; b; b = b->next)
                if (b->pos.x == x && b->pos.y == y)
                        return 1;
        return 0;
}

void init_beast(struct level *l, const struct hunter *h)
{
        int ch, no;
        struct room *r;
        for (ch = 'A'; ch <= 'D'; ch++) {
                for (no = 1; no <= 4; no++) {
                        r = get_room_by_idx(l->r, ch, no);
                        if (r)
                                add_beast(&l->b, r->tl.x+1, r->tl.y+1);
                }
        }
}

void free_beast(struct beast *b)
{
        struct beast *t;
        while (b) {
                t = b;
                b = b->next;
                free(t);
        }
}

static void move_beast(struct beast *b, int dx, int dy)
{
        b->pos.x += dx;
        b->pos.y += dy;
}

/* Return -1 when the search fails */
static int search_hunter(struct beast *b, struct hunter *h)
{
        if (h->pos.x >= b->pos.x - b->fov && h->pos.x <= b->pos.x + b->fov &&
            h->pos.y >= b->pos.y - b->fov && h->pos.y <= b->pos.y + b->fov) {
                if (h->pos.x < b->pos.x && h->pos.y < b->pos.y)
                        return side_northwest;
                if (h->pos.x == b->pos.x && h->pos.y < b->pos.y)
                        return side_north;
                if (h->pos.x > b->pos.x && h->pos.y < b->pos.y)
                        return side_northeast;
                if (h->pos.x > b->pos.x && h->pos.y == b->pos.y)
                        return side_east;
                if (h->pos.x > b->pos.x && h->pos.y > b->pos.y)
                        return side_southeast;
                if (h->pos.x == b->pos.x && h->pos.y > b->pos.y)
                        return side_south;
                if (h->pos.x < b->pos.x && h->pos.y > b->pos.y)
                        return side_southwest;
                if (h->pos.x < b->pos.x && h->pos.y == b->pos.y)
                        return side_west;
        }
        return -1;
}

static int try_side(struct level *l, int side, int x, int y)
{
/* TODO upgrade */
        return side == -1 ? rand() % 8 : side;
}

void handle_beast(struct level *l, struct hunter *h)
{
        int side;
        struct beast *b;
        for (b = l->b; b; b = b->next) {
                side = search_hunter(b, h);
                side = try_side(l, side, b->pos.x, b->pos.y);
                switch (side) {
                case side_northwest:
                        if (can_move(l, h, b->pos.x-1, b->pos.y-1))
                                move_beast(b, -1, -1);
                        break;
                case side_north:
                        if (can_move(l, h, b->pos.x, b->pos.y-1))
                                move_beast(b, 0, -1);
                        break;
                case side_northeast:
                        if (can_move(l, h, b->pos.x+1, b->pos.y-1))
                                move_beast(b, 1, -1);
                        break;
                case side_east:
                        if (can_move(l, h, b->pos.x+1, b->pos.y))
                                move_beast(b, 1, 0);
                        break;
                case side_southeast:
                        if (can_move(l, h, b->pos.x+1, b->pos.y+1))
                                move_beast(b, 1, 1);
                        break;
                case side_south:
                        if (can_move(l, h, b->pos.x, b->pos.y+1))
                                move_beast(b, 0, 1);
                        break;
                case side_southwest:
                        if (can_move(l, h, b->pos.x-1, b->pos.y+1))
                                move_beast(b, -1, 1);
                        break;
                case side_west:
                        if (can_move(l, h, b->pos.x-1, b->pos.y))
                                move_beast(b, -1, 0);
                        break;
                }
        }
}

void show_beast(const struct beast *b)
{
        for ( ; b; b = b->next)
                mvwaddch(gamew, b->pos.y, b->pos.x, b->symb);
}
