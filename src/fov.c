/* fov - fov system */

#include "rogue.h"

void handle_fov(const struct hunter *h, const struct level *l)
{
        show_rooms(l->r, l->d);
        show_path(l->p);
        show_points(l->start, l->end);
        show_hunter(h);
}
