#include <stdlib.h>
#include <time.h>
#include <curses.h>

struct hunter {
        int symb;
        int cur_x, cur_y;
};

struct room {
        int tl_x, tl_y, br_x, br_y;
        int depth;
        struct room *parent, *left, *right;
};

struct level {
        struct room *r;
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
        min_room_len = 5,
        min_room_len_to_split = min_room_len * 2 - 1,

        split_type_ver = 0,
        split_type_hor,

        hunter_symb = '@',
        door_symb   = '+'
};

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

static int room_length(const struct room *r, char dir)
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

static void init_room(struct room **r)
{
        struct room *t;
        t = malloc(sizeof(*t));
        t->tl_x = 0;
        t->tl_y = 0;
        t->br_x = gamew_col-1;
        t->br_y = gamew_row-1;
        t->depth = 0;
        t->parent = NULL;
        t->left = NULL;
        t->right = NULL;
        *r = t;
}

static int get_split_type(struct room *r)
{
        if (room_length(r, 'h') > min_room_len_to_split &&
            room_length(r, 'v') > min_room_len_to_split)
                return rand() % 2;
        else
        if (room_length(r, 'h') > min_room_len_to_split &&
            room_length(r, 'v') < min_room_len_to_split)
                return split_type_hor;
        else
        if (room_length(r, 'h') < min_room_len_to_split &&
            room_length(r, 'v') > min_room_len_to_split)
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
        type = get_split_type(r);
        if (type == -1)
                return 0;
        init_split_children(r);
        switch (type) {
        case split_type_hor:
                /*
                 * r->tl_* + (min_room_len - 1) guarantees at least 
                 * (min_room_len - 2) empty spaces for rooms 
                 *
                 * rand() % (room_length(r, *) - min_room_len_to_split + 1)
                 * mean if we`ve more len than min len to split then we`ve
                 * chance to build bigger room
                 */
                split = r->tl_x + (min_room_len - 1) + rand() %
                        (room_length(r, 'h') - min_room_len_to_split + 1);
                r->left->br_x = split;
                r->right->tl_x = split;
                break;
        case split_type_ver:
                split = r->tl_y + (min_room_len - 1) + rand() %
                        (room_length(r, 'v') - min_room_len_to_split + 1);
                r->left->br_y = split;
                r->right->tl_y = split;
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

static void init_level(struct level *l)
{
        int res, i, depth;
        init_room(&l->r);
        depth = 1 + rand() % room_splits_range;
        for (i = 0; i < depth; i++) {
                res = split_room(l->r);
                if (!res) {
                        free_depth(&l->r, depth);
                        break;
                }
        }
}

static void end_level(struct level *l)
{
        free_room(l->r);
}

static void show_hunter(const struct hunter *h)
{
        mvwaddch(gamew, h->cur_y, h->cur_x, h->symb);
        wrefresh(gamew);
}

static void show_room(const struct room *r)
{
        int x, y;
        for (y = r->tl_y; y <= r->br_y; y++) {
                mvwaddch(gamew, y, r->tl_x, '#');
                mvwaddch(gamew, y, r->br_x, '#');
                if (y == r->tl_y || y == r->br_y)
                        for (x = r->tl_x + 1; x < r->br_x; x++)
                                mvwaddch(gamew, y, x, '#');
        }
        wrefresh(gamew);
}

static void show_rooms(const struct room *r)
{
        if (!r)
                return;
        if (r->left)
                show_rooms(r->left);
        if (!r->left) {
                show_room(r);
#ifdef DEBUG
                fprintf(logfile, "depth: %d\ntl: %d-%d\nbr: %d-%d\n\n",
                        r->depth, r->tl_x, r->tl_y, r->br_x, r->br_y);
                fflush(logfile);
#endif
                return;
        }
        if (r->right)
                show_rooms(r->right);
}

static void handle_fov(const struct hunter *h, const struct level *l)
{
        struct room *r;
        show_hunter(h);
        show_rooms(l->r);
        /*
        for (r = l->r; r; r = r->next) {
                if (r->tl_x < h->cur_x && r->br_x > h->cur_x &&
                    r->tl_y < h->cur_y && r->br_y > h->cur_y) {
                        show_room(r);
                        break;
                }
        }
        */
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

/* DEBUG windows */
        mvwprintw(msgw, 0, 0, "hello everyone");
        wrefresh(msgw);
        mvwprintw(infow, 0, 0, "Info window");
        wrefresh(infow);
/* /DEBUG */

        for (;;) {
                init_level(&l);
                play_game(&h, &l);
                end_level(&l);
                break;
        }
        end_game();
        return 0;
}
