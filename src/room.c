/* room.c - room generating */

#include <stdlib.h>
#include <curses.h>

#include "rogue.h"

enum {
    room_splits_range = 4,
    min_area_len = 4,
    min_area_len_to_split = min_area_len * 2 + 1,

    split_type_ver = 0,
    split_type_hor
};

int room_len(const struct room *r, char dir)
{
    if (!r)
        return 0;
    switch (dir) {
    case 'h':
        return r->br.x - r->tl.x + 1;
    case 'v':
        return r->br.y - r->tl.y + 1;
    default:
        return 0;
    }
}

static int rand_split_type(const struct room *r)
{
    if (room_len(r, 'h') > min_area_len_to_split &&
        room_len(r, 'v') > min_area_len_to_split)
        return rand() % 2;
    else
    if (room_len(r, 'h') > min_area_len_to_split &&
        room_len(r, 'v') < min_area_len_to_split)
        return split_type_hor;
    else
    if (room_len(r, 'h') < min_area_len_to_split &&
        room_len(r, 'v') > min_area_len_to_split)
        return split_type_ver;
    else
        return -1;
}

static void init_split_children(struct room *r)
{
    r->left         = malloc(sizeof(*r));
    *(r->left)      = *r;
    r->left->depth++;
    r->left->parent = r;
    r->left->left   = NULL;
    r->left->right  = NULL;
    r->right        = malloc(sizeof(*r));
    *(r->right)     = *r->left;
}

static int split_room(struct room *r)
{
    int res, split, type;
    if (r->left) {
        res = split_room(r->left);
        if (!res)
            return 0;
        res = split_room(r->right);
        if (!res)
            return 0;
        return 1;
    }
    type = rand_split_type(r);
    if (type == -1)
        return 0;
    init_split_children(r);
    switch (type) {
    case split_type_hor:
        /*
         * r->tl.* + (min_area_len - 1) guarantees at least
         * (min_area_len - 2) empty spaces for rooms
         *
         * rand() % (room_len(r, *) - min_area_len_to_split + 1)
         * mean if we`ve more len than min len to split then we`ve
         * chance to build bigger room
         */
        split = r->tl.x + (min_area_len - 1) + rand() %
                (room_len(r, 'h') - min_area_len_to_split + 1);
        r->left->br.x = split;
        r->right->tl.x = split;
        /* Make empty space between areas */
        if (room_len(r->left, 'h') > room_len(r->right, 'h'))
            r->left->br.x -= 2;
        else
            r->right->tl.x += 2;
        break;
    case split_type_ver:
        split = r->tl.y + (min_area_len - 1) + rand() %
                (room_len(r, 'v') - min_area_len_to_split + 1);
        r->left->br.y = split;
        r->right->tl.y = split;
        if (room_len(r->left, 'v') > room_len(r->right, 'v'))
            r->left->br.y -= 2;
        else
            r->right->tl.y += 2;
        break;
    }
    return 1;
}

void free_room(struct room *r)
{
    if (!r)
        return;
    if (r->left)
        free_room(r->left);
    if (r->right)
        free_room(r->right);
    free_linked_coord(r->seen_walls);
    free(r);
}

static void free_depth(struct room **r, int depth)
{
    if (!*r)
        return;
    if ((*r)->depth == depth) {
        free_room(*r);
        *r = NULL;
        return;
    }
    if ((*r)->left)
        free_depth(&(*r)->left, depth);
    if ((*r)->right)
        free_depth(&(*r)->right, depth);
}

static void polish_room(struct room *r)
{
    if (r->left) {
        polish_room(r->left);
        polish_room(r->right);
        return;
    }
    r->tl.x = r->tl.x + rand() % (room_len(r, 'h') - min_area_len + 1);
    r->br.x = r->tl.x + (min_area_len - 1) + rand() %
              (room_len(r, 'h') - (min_area_len - 1));
    r->tl.y = r->tl.y + rand() % (room_len(r, 'v') - min_area_len + 1);
    r->br.y = r->tl.y + (min_area_len - 1) + rand() %
              (room_len(r, 'v') - (min_area_len - 1));
}

static int give_no_idx(struct room *r, int depth, int no_idx)
{
    if (r->left) {
        no_idx = give_no_idx(r->left, depth, no_idx);
    } else {
        r->no_idx = no_idx;
        if (no_idx >= (depth <= 2 ? 2 : 4))
            return 1;
        return no_idx + 1;
    }
    if (r->right)
        no_idx = give_no_idx(r->right, depth, no_idx);
    return no_idx;
}

static int give_ch_idx(struct room *r, int depth, int ch_idx)
{
    if (r->left) {
        ch_idx = give_ch_idx(r->left, depth, ch_idx);
    } else {
        r->ch_idx = ch_idx;
        if (r->no_idx >= (depth <= 2 ? 2 : 4))
            return ch_idx + 1;
        return ch_idx;
    }
    if (r->right)
        ch_idx = give_ch_idx(r->right, depth, ch_idx);
    return ch_idx;
}

static void give_idx(struct room *r, int depth)
{
    give_no_idx(r, depth, 1);
    give_ch_idx(r, depth, 'A');
}

int init_room(struct level *l)
{
    int i, res, depth;
    struct room *t;
    t = malloc(sizeof(*t));
    t->ch_idx     = 0;
    t->no_idx     = 0;
    t->tl.x       = 0;
    t->tl.y       = 0;
    t->br.x       = gamew_col-1;
    t->br.y       = gamew_row-1;
    t->depth      = 0;
    t->parent     = NULL;
    t->left       = NULL;
    t->right      = NULL;
    t->seen_walls = NULL;
    t->flags      = 0;
    l->r = t;

    for (i = 1; i <= l->lt->depth; i++) {
        res = split_room(l->r);
        if (!res) {
            free_depth(&l->r, i);
            break;
        }
    }
    depth = i - 1;
    polish_room(l->r);
    give_idx(l->r, depth);
    return depth;
}

struct room *get_room_by_idx(const struct room *r, int ch_idx, int no_idx)
{
    struct room *t;
    if (r->left) {
        t = get_room_by_idx(r->left, ch_idx, no_idx);
        if (t)
            return t;
    } else {
        if (r->ch_idx == ch_idx && r->no_idx == no_idx)
            return (struct room *)r;
        return NULL;
    }
    if (r->right)
        t = get_room_by_idx(r->right, ch_idx, no_idx);
    return t;
}

struct room *get_room_by_coord(const struct room *r, int x, int y)
{
    struct room *t;
    if (r->left) {
        t = get_room_by_coord(r->left, x, y);
        if (t)
            return t;
    } else {
        if (x >= r->tl.x && x <= r->br.x &&
            y >= r->tl.y && y <= r->br.y)
            return (struct room *)r;
        return NULL;
    }
    if (r->right)
        t = get_room_by_coord(r->right, x, y);
    return t;
}

struct room *get_random_room(const struct room *r)
{
    if (!r->left)
        return (struct room *)r;
    return get_random_room(rand() % 2 ? r->left : r->right);
}

int is_room(const struct room *r, int x, int y)
{
    return get_room_by_coord(r, x, y) ? 1 : 0;
}

int is_one_room(const struct room *r, struct coord pos1, struct coord pos2)
{
    if (get_room_by_coord(r, pos1.x, pos1.y) ==
        get_room_by_coord(r, pos2.x, pos2.y))
        return 1;
    return 0;
}

int is_wall(const struct room *r, int x, int y)
{
    r = get_room_by_coord(r, x, y);
    if (r)
        if ((x >= r->tl.x && x <= r->br.x &&
            (y == r->tl.y || y == r->br.y)) ||
            (y >= r->tl.y && y <= r->br.y &&
            (x == r->tl.x || x == r->br.x)))
            return 1;
    return 0;
}

void show_room(struct room *r, const struct door *d, int ground)
{
    int x, y, symb;
    for (y = r->tl.y; y <= r->br.y; y++) {
        if (y == r->tl.y || y == r->br.y) {
            mvwaddch(gamew, y, r->tl.x, hor_wall_symb);
            mvwaddch(gamew, y, r->br.x, hor_wall_symb);
        } else {
            mvwaddch(gamew, y, r->tl.x, ver_wall_symb);
            mvwaddch(gamew, y, r->br.x, ver_wall_symb);
        }
        if (y == r->tl.y || y == r->br.y)
                symb = hor_wall_symb;
        else if (ground)
                symb = ground_symb;
        else
                symb = ' ';
        for (x = r->tl.x + 1; x < r->br.x; x++)
                mvwaddch(gamew, y, x, symb);
    }
    for ( ; d; d = d->next)
        if (d->owner == r)
            mvwaddch(gamew, d->pos.y, d->pos.x, door_symb);
    if (r->flags & see_flag && !ground)
        r->flags ^= see_flag;
    else if (~r->flags & see_flag && ground)
        r->flags |= see_flag;
}

void show_rooms(struct room *r, const struct door *d)
{
    if (!r)
        return;
    if (r->left) {
        show_rooms(r->left, d);
    } else {
        show_room(r, d, 1);
        return;
    }
    if (r->right)
        show_rooms(r->right, d);
}

int is_seen_wall(const struct linked_coord *sw, int x, int y)
{
    for ( ; sw; sw = sw->next)
        if (sw->pos.x == x && sw->pos.y == y)
            return 1;
    return 0;
}

void show_seen_walls(const struct room *r, const struct door *d)
{
    struct linked_coord *sw;
    for (sw = r->seen_walls; sw; sw = sw->next)
        if (is_door(d, sw->pos.x, sw->pos.y))
            mvwaddch(gamew, sw->pos.y, sw->pos.x, door_symb);
        else if (sw->pos.y == r->tl.y || sw->pos.y == r->br.y)
            mvwaddch(gamew, sw->pos.y, sw->pos.x, hor_wall_symb);
        else
            mvwaddch(gamew, sw->pos.y, sw->pos.x, ver_wall_symb);
}
