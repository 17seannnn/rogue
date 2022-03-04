/* main.c - main game functions */

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

static void play_game(struct level *l, struct creature *h)
{
        int c, again = 0;
        handle_fov(l, h);
        show_info(h);
        handle_msg();
        while (again || (c = wgetch(gamew)) != 27) {
                again = do_cmd(c, h, l);
                handle_beast(l, h);
                if (again && !msg)
                        continue;
                else
                        again = 0;
                handle_fov(l, h);
                show_info(h);
                handle_msg();
        }
}

int main()
{
        struct creature h;
        struct level l;
        init_game();
        for (;;) {
                init_level(&l, &h);
                play_game(&l, &h);
                end_level(&l);
                break;
        }
        end_game();
        return 0;
}
