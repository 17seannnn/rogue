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

void move_beast(struct beast *b, int dx, int dy)
{
        b->pos.x += dx;
        b->pos.y += dy;
}

void handle_beast(struct level *l, struct hunter *h)
{
        int side;
        struct beast *b;
        for (b = l->b; b; b = b->next) {
                side = side_northwest + rand() % 8;
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
