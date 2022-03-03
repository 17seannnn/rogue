/* fov.c - fov system */

#include "rogue.h"

void handle_fov(const struct level *l, const struct creature *h)
{
        const struct beast *b;
        show_rooms(l->r, l->d);
        show_path(l->p);
        show_points(l->start, l->end);
        show_creature(h);
        for (b = l->b; b; b = b->next)
                show_creature(&b->c);
}
