#include <stdlib.h>
#include <time.h>
#include <curses.h>

struct hunter {
        int symb;
        int cur_x, cur_y;
};

struct room {
        int ch_idx, no_idx;
        int tl_x, tl_y, br_x, br_y;
        int depth;
        struct room *parent, *left, *right;
};

struct path {
        int cur_x, cur_y;
        struct path *next;
};

struct door {
        int cur_x, cur_y;
        struct room *owner;
        struct door *next;
};

struct level {
        int depth;
        struct room *r;
        struct path *p;
        struct door *d;
};

#ifdef DEBUG
FILE *logfile;
#endif

WINDOW *msgw, *gamew, *infow;

enum {
        msgw_col  = 80,
        msgw_row  = 1,
        gamew_col = 80,
        gamew_row = 22,
        infow_col = 80,
        infow_row = 1,

        room_splits_range = 4,
        min_area_len = 5,
        min_area_len_to_split = min_area_len * 2 + 1,

        split_type_ver = 0,
        split_type_hor,

        pave_steps_range = 10,

        hunter_symb = '@',
        door_symb   = '+'
};

static int abs_int(int i)
{
        return i < 0 ? i * -1 : i;
}

static void init_curses()
{
        initscr();
        msgw  = newwin(msgw_row, msgw_col, 0, 0);
        gamew = newwin(gamew_row, gamew_col, msgw_row, 0);
        infow = newwin(infow_row, infow_col, msgw_row + gamew_row, 0);
        if (has_colors()) {
                start_color();
                use_default_colors();
        }
        noecho();
        cbreak();
        curs_set(0);
        keypad(gamew, 1);
        timeout(-1);
}

static void end_curses()
{
        endwin();
}

static void init_hunter(struct hunter *h)
{
        h->symb = hunter_symb;
        h->cur_x = 0;
        h->cur_y = 0;
}

static void init_game(struct hunter *h)
{
#ifdef DEBUG
        logfile = fopen("log", "w");
#endif
        srand(time(NULL));
        init_curses();
        init_hunter(h);
}

static void end_game()
{
        end_curses();
}

static int is_ok(int x, int y)
{
        return x >= 0 && x < gamew_col && y >= 0 && y < gamew_row;
}

static int room_len(const struct room *r, char dir)
{
        if (!r)
                return 0;
        switch (dir) {
        case 'h':
                return r->br_x - r->tl_x + 1;
        case 'v':
                return r->br_y - r->tl_y + 1;
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
        r->left = malloc(sizeof(*r));
        *r->left = *r;
        r->left->depth++;
        r->left->parent = r;
        r->left->left = NULL;
        r->left->right = NULL;
        r->right = malloc(sizeof(*r));
        *r->right = *r->left;
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
                 * r->tl_* + (min_area_len - 1) guarantees at least
                 * (min_area_len - 2) empty spaces for rooms
                 *
                 * rand() % (room_len(r, *) - min_area_len_to_split + 1)
                 * mean if we`ve more len than min len to split then we`ve
                 * chance to build bigger room
                 */
                split = r->tl_x + (min_area_len - 1) + rand() %
                        (room_len(r, 'h') - min_area_len_to_split + 1);
                r->left->br_x = split;
                r->right->tl_x = split;
                /* Make empty space between areas */
                if (room_len(r->left, 'h') > room_len(r->right, 'h'))
                        r->left->br_x -= 2;
                else
                        r->right->tl_x += 2;
                break;
        case split_type_ver:
                split = r->tl_y + (min_area_len - 1) + rand() %
                        (room_len(r, 'v') - min_area_len_to_split + 1);
                r->left->br_y = split;
                r->right->tl_y = split;
                if (room_len(r->left, 'v') > room_len(r->right, 'v'))
                        r->left->br_y -= 2;
                else
                        r->right->tl_y += 2;
                break;
        }
        return 1;
}

static void free_room(struct room *r)
{
        if (!r)
                return;
        if (r->left)
                free_room(r->left);
        if (r->right)
                free_room(r->right);
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
        r->tl_x = r->tl_x + rand() % (room_len(r, 'h') - min_area_len + 1);
        r->br_x = r->tl_x + (min_area_len - 1) + rand() %
                  (room_len(r, 'h') - (min_area_len - 1));
        r->tl_y = r->tl_y + rand() % (room_len(r, 'v') - min_area_len + 1);
        r->br_y = r->tl_y + (min_area_len - 1) + rand() %
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

static int init_room(struct room **r)
{
        int i, res, depth;
        struct room *t;
        t = malloc(sizeof(*t));
        t->ch_idx = 0;
        t->no_idx = 0;
        t->tl_x = 0;
        t->tl_y = 0;
        t->br_x = gamew_col-1;
        t->br_y = gamew_row-1;
        t->depth = 0;
        t->parent = NULL;
        t->left = NULL;
        t->right = NULL;
        *r = t;

        depth = 1 + rand() % room_splits_range;
        for (i = 1; i <= depth; i++) {
                res = split_room(*r);
                if (!res) {
                        free_depth(r, i);
                        break;
                }
        }
        depth = i - 1;
        polish_room(*r);
        give_idx(*r, depth);
        return depth;
}

static struct room *get_room_by_idx(const struct room *r, int ch_idx,
                                                          int no_idx)
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

static struct room *get_room_by_coord(const struct room *r, int x, int y)
{
        struct room *t;
        if (r->left) {
                t = get_room_by_coord(r->left, x, y);
                if (t)
                        return t;
        } else {
                if (x >= r->tl_x && x <= r->br_x &&
                    y >= r->tl_y && y <= r->br_y)
                        return (struct room *)r;
                return NULL;
        }
        if (r->right)
                t = get_room_by_coord(r->right, x, y);
        return t;
}

static int is_room(const struct room *r, int x, int y)
{
        r = get_room_by_coord(r, x, y);
        return r ? 1 : 0;
}

static int is_wall(struct room *r, int x, int y)
{
        r = get_room_by_coord(r, x, y);
        if (r)
                if ((x >= r->tl_x && x <= r->br_x &&
                    (y == r->tl_y || y == r->br_y)) ||
                    (y >= r->tl_y && y <= r->br_y &&
                    (x == r->tl_x || x == r->br_x)))
                        return 1;
        return 0;
}

static void add_path(struct path **p, int x, int y)
{
        struct path *t;
        t = malloc(sizeof(t));
        t->cur_x = x;
        t->cur_y = y;
        t->next = *p;
        *p = t;
}

static int is_path(struct path *p, int x, int y)
{
        for ( ; p; p = p->next)
                if (p->cur_x == x && p->cur_y == y)
                        return 1;
        return 0;
}

static void place_door(struct door **d, struct room *owner, int x, int y)
{
        for ( ; *d; d = &(*d)->next)
                {}
        *d = malloc(sizeof(**d));
        (*d)->cur_x = x;
        (*d)->cur_y = y;
        (*d)->owner = owner;
        (*d)->next = NULL;
}

static int is_door(struct door *d, int x, int y)
{
        for ( ; d; d = d->next)
                if (d->cur_x == x && d->cur_y == y)
                        return 1;
        return 0;
}

static int can_pave(struct level *l, int x, int y)
{
        return is_ok(x, y) && !is_room(l->r, x, y) &&
               !is_path(l->p, x, y) && !is_door(l->d, x, y);
}

static int pave_hor(struct level *l, int b_x, int b_y, int e_x, int e_y)
{
        int steps;
        steps = rand() % pave_steps_range + 1;
        if (steps > abs_int(e_x - b_x) + 1)
                steps = abs_int(e_x - b_x) + 1;
        for ( ; steps > 0; steps--) {
                if (can_pave(l, b_x, b_y)) {
                        add_path(&l->p, b_x, b_y);
                } else {
                }
                if (e_x - b_x > 0)
                        b_x++;
                else
                if (e_x - b_x < 0)
                        b_x--;
                else
                        break;
        }
        return b_x;
}

static int pave_ver(struct level *l, int b_x, int b_y, int e_x, int e_y)
{
        int steps;
        steps = rand() % pave_steps_range + 1;
        if (steps > abs_int(e_y - b_y) + 1)
                steps = abs_int(e_y - b_y) + 1;
        for ( ; steps > 0; steps--) {
                if (can_pave(l, b_x, b_y)) {
                        add_path(&l->p, b_x, b_y);
                } else {
                }
                if (e_y - b_y > 0)
                        b_y++;
                else
                if (e_y - b_y < 0)
                        b_y--;
                else
                        break;
        }
        return b_y;
}

static void pave_path(struct level *l, struct room *r1, struct room *r2)
{
        int order;
        int b_x, b_y, e_x, e_y;
        b_x = r1->tl_x + rand() % room_len(r1, 'h');
        b_y = r1->tl_y + rand() % room_len(r1, 'v');
        e_x = r2->tl_x + rand() % room_len(r2, 'h');
        e_y = r2->tl_y + rand() % room_len(r2, 'v');
        order = rand() % 2;
        for (;;) {
                if (order) {
                        b_x = pave_hor(l, b_x, b_y, e_x, e_y);
                        b_y = pave_ver(l, b_x, b_y, e_x, e_y);
                } else {
                        b_y = pave_ver(l, b_x, b_y, e_x, e_y);
                        b_x = pave_hor(l, b_x, b_y, e_x, e_y);
                }
                if (b_x == e_x && b_y == e_y)
                        break;
        }
}

static void init_path(struct level *l)
{
        int c, n, next_c, next_n;
        struct room *r1, *r2;
        l->p = NULL;
        l->d = NULL;
        c = 'A';
        n = 1;
        next_c = c;
        next_n = n + 1;
        while ((r1 = get_room_by_idx(l->r, c, n)) &&
               (r2 = get_room_by_idx(l->r, next_c, next_n))) {
                pave_path(l, r1, r2);
                c = next_c;
                n = next_n;
                if (next_n >= (l->depth <= 2 ? 2 : 4)) {
                        next_c++;
                        next_n = 1;
                } else {
                        next_n++;
                }
        }
}

static void free_path(struct path *p)
{
        struct path *t;
        while (p) {
                t = p;
                p = p->next;
                free(t);
        }
}

static void free_door(struct door *d)
{
        struct door *t;
        while (d) {
                t = d;
                d = d->next;
                free(t);
        }
}

static void init_level(struct level *l)
{
        l->depth = init_room(&l->r);
        init_path(l);
}

static void end_level(struct level *l)
{
        free_room(l->r);
        free_path(l->p);
        free_door(l->d);
}

static void show_hunter(const struct hunter *h)
{
        mvwaddch(gamew, h->cur_y, h->cur_x, h->symb);
        wrefresh(gamew);
}

static void show_room(const struct room *r, const struct door *d)
{
        int x, y;
        for (y = r->tl_y; y <= r->br_y; y++) {
                mvwaddch(gamew, y, r->tl_x, '#');
                mvwaddch(gamew, y, r->br_x, '#');
                if (y == r->tl_y || y == r->br_y)
                        for (x = r->tl_x + 1; x < r->br_x; x++)
                                mvwaddch(gamew, y, x, '#');
        }
        for ( ; d; d = d->next) {
                if (d->owner == r)
                        mvwaddch(gamew, d->cur_y, d->cur_x, '+');
        }
        wrefresh(gamew);
}

static void show_rooms(const struct room *r, const struct door *d)
{
        if (!r)
                return;
        if (r->left)
                show_rooms(r->left, d);
        if (!r->left) {
                show_room(r, d);
                return;
        }
        if (r->right)
                show_rooms(r->right, d);
}

static void show_path(struct path *p)
{
        for ( ; p; p = p->next)
                mvwaddch(gamew, p->cur_y, p->cur_x, '"');
        wrefresh(gamew);
}

static void handle_fov(const struct hunter *h, const struct level *l)
{
        show_rooms(l->r, l->d);
        show_path(l->p);
        show_hunter(h);
}

static void move_hunter(struct hunter *h, int c)
{
        switch (c) {
                case 'h':
                case KEY_LEFT:
                        h->cur_x -= 1;
                        break;
                case 'j':
                case KEY_DOWN:
                        h->cur_y += 1;
                        break;
                case 'k':
                case KEY_UP:
                        h->cur_y -= 1;
                        break;
                case 'l':
                case KEY_RIGHT:
                        h->cur_x += 1;
                        break;
                default:
                        break;
        }
}

static void do_cmd(int c, struct hunter *h)
{
                switch (c) {
                case 'h': case 'j': case 'k': case 'l':
                case KEY_LEFT: case KEY_DOWN: case KEY_UP: case KEY_RIGHT:
                        move_hunter(h, c);
                        break;
                default:
                        break;
                }
}

static void play_game(struct hunter *h, struct level *l)
{
        int c;
        handle_fov(h, l);
        while ((c = wgetch(gamew)) != 27) {
                do_cmd(c, h);
                handle_fov(h, l);
        }
}

int main(int argc, char **argv)
{
        struct hunter h;
        struct level l;
        init_game(&h);
#ifdef DEBUG
        mvwprintw(msgw, 0, 0, "hello everyone");
        wrefresh(msgw);
        mvwprintw(infow, 0, 0, "Info window");
        wrefresh(infow);
#endif
        for (;;) {
                init_level(&l);
                play_game(&h, &l);
                end_level(&l);
                break;
        }
        end_game();
        return 0;
}
