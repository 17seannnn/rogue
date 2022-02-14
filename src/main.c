#include <curses.h>

struct hunter {
        int symb;
        int cur_x, cur_y;
};

WINDOW *msgw, *gamew, *infow;

enum {
        hunter_symb = '@'
};

static void init_curses()
{
        initscr();
        if (has_colors()) {
                start_color();
                use_default_colors();
        }
        noecho();
        cbreak();
        curs_set(0);
        keypad(stdscr, 1);
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
        mvaddch(h.cur_y, h.cur_x, h.symb);
        refresh();
}

static void handle_fov(struct hunter h)
{
        clear();
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
        while ((c = getch()) != 27) {
                do_cmd(c, h);
                handle_fov(*h);
        }
}

int main(int argc, char **argv)
{
        struct hunter h;
        init_game(&h);
        play_game(&h);
        end_game();
        return 0;
}
