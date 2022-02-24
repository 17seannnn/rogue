#include <stdlib.h>
#include <time.h>

#include "rogue.h"

#ifdef DEBUG
FILE *logfile;
#endif

static void init_game()
{
#ifdef DEBUG
        logfile = fopen("log", "w");
#endif
        srand(time(NULL));
        init_curses();
}

static void end_game()
{
        end_curses();
}

static void play_game(struct level *l, struct hunter *h)
{
        int c;
        handle_fov(l, h);
        while ((c = wgetch(gamew)) != 27) {
                do_cmd(c, h);
                handle_fov(l, h);
        }
}

int main(int argc, char **argv)
{
        struct hunter h;
        struct level l;
        init_game();
#ifdef DEBUG
        mvwprintw(msgw, 0, 0, "hello everyone");
        wrefresh(msgw);
        mvwprintw(infow, 0, 0, "Info window");
        wrefresh(infow);
#endif
        for (;;) {
                init_level(&l, &h);
                play_game(&l, &h);
                end_level(&l);
                break;
        }
        end_game();
        return 0;
}
