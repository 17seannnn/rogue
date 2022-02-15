#include <stdlib.h>
#include <time.h>
#include <curses.h>

struct hunter {
        int symb;
        int cur_x, cur_y;
};

struct room {
        int ul_x, ul_y, br_x, br_y;
        struct room *next;
};

struct level {
        struct room *r;
};

FILE *logfile;
WINDOW *msgw, *gamew, *infow;

enum {
        msgw_col  = 80,
        msgw_row  = 1,
        gamew_col = 80,
        gamew_row = 22,
        infow_col = 80,
        infow_row = 1,

        room_range = 12,

        hunter_symb = '@'
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
        logfile = fopen("log", "a");
        srand(time(NULL));
        init_curses();
        init_hunter(h);
}

static void end_game()
{
        end_curses();
}

static int fix_room(struct room *n, struct room *r)
{
        return 1;
}

static int create_room(struct room **r)
{
        int res;
        struct room n;
        /* -2 set limit for room */
        n.ul_x = rand() % (gamew_col - 2);
        n.ul_y = rand() % (gamew_row - 2);
        /*
         * + 2 for empty space,
         * - ul_x - 2 set limit for room
         */
        n.br_x = n.ul_x + 2 + rand() % (gamew_col - n.ul_x - 2);
        n.br_y = n.ul_y + 2 + rand() % (gamew_row - n.ul_y - 2);
        n.next = NULL;

        res = fix_room(&n, *r);
        if (!res)
                return 0;

        for ( ; *r; r = &(*r)->next)
                {}
        *r = malloc(sizeof(**r));
        **r = n;

        fprintf(logfile, "ul: %d-%d\nbr: %d-%d\n\n",
                n.ul_x, n.ul_y, n.br_x, n.br_y);

        return 1;
}

static void init_level(struct level *l)
{
        int res, count;
        l->r = NULL;
        for (count = rand() % room_range + 1; count; count--) {
                res = create_room(&l->r);
                if (!res)
                        return;
        }
}

static void end_level(struct level *l)
{
        struct room *t;
        while (l->r) {
                t = l->r;
                l->r = l->r->next;
                free(t);
        }
}

static void show_hunter(struct hunter *h)
{
        mvwaddch(gamew, h->cur_y, h->cur_x, h->symb);
        wrefresh(gamew);
}

static void show_room(struct room *r)
{
        int x, y;
        for (y = r->ul_y; y <= r->br_y; y++) {
                mvwaddch(gamew, y, r->ul_x, '#');
                mvwaddch(gamew, y, r->br_x, '#');
                if (y == r->ul_y || y == r->br_y)
                        for (x = r->ul_x + 1; x < r->br_x; x++)
                                mvwaddch(gamew, y, x, '#');
        }
        wrefresh(gamew);
}

static void handle_fov(struct hunter *h, struct level *l)
{
        struct room *r;
        show_hunter(h);
        for (r = l->r; r; r = r->next) {
                show_room(r);
        }
        /*
        for (r = l->r; r; r = r->next) {
                if (r->ul_x < h->cur_x && r->br_x > h->cur_x &&
                    r->ul_y < h->cur_y && r->br_y > h->cur_y) {
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
