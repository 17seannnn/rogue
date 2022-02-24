/* fov - fov system */

#include "rogue.h"

void handle_fov(const struct level *l, const struct hunter *h)
{
        show_rooms(l->r, l->d);
        show_path(l->p);
        show_points(l->start, l->end);
        show_hunter(h);
}
