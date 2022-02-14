#include <curses.h>

struct hunter {
        int symb;
        int cur_x, cur_y;
};

WINDOW *msgw, *gamew, *infow;

enum {
        msgw_col  = 80,
        msgw_row  = 1,
        gamew_col = 80,
        gamew_row = 22,
        infow_col = 80,
        infow_row = 1,

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
        init_curses();
        init_hunter(h);
}

static void end_game()
{
        end_curses();
}

static void show_hunter(struct hunter h)
{
        mvwaddch(gamew, h.cur_y, h.cur_x, h.symb);
        wrefresh(gamew);
}

static void handle_fov(struct hunter h)
{
        wclear(gamew);
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

static void play_game(struct hunter *h)
{
        int c;
        handle_fov(*h);
        while ((c = wgetch(gamew)) != 27) {
                do_cmd(c, h);
                handle_fov(*h);
        }
}

int main(int argc, char **argv)
{
        struct hunter h;
        init_game(&h);

/* DEBUG windows */
        mvwprintw(msgw, 0, 0, "hello everyone");
        wrefresh(msgw);
        mvwprintw(infow, 0, 0, "Info window");
        wrefresh(infow);
/* /DEBUG */

        play_game(&h);
        end_game();
        return 0;
}
