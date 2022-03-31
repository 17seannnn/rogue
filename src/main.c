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

static unsigned play_game(struct level *l, struct creature *h)
{
        int c;
        unsigned flags = 0;
        handle_fov(l, h);
        show_info(h);
        handle_msg();
        for (;;) {
                if (!(flags & again)) {
                        c = wgetch(gamew);
                        if (c == key_escape)
                                break;
                }
                do_cmd(c, h, l, &flags);
                handle_beast(l, h);
                if (flags & next_level)
                        break;
                if (flags & again && !msg)
                        continue;
                else
                if (flags & again && msg)
                        flags ^= again;
                handle_fov(l, h);
                show_info(h);
                handle_msg();
        }
        return flags;
}

int main()
{
        unsigned flags;
        struct creature h;
        struct level l;
        init_game();
        for (;;) {
                init_level(&l, &h);
                flags = play_game(&l, &h);
                end_level(&l);
                if (!(flags & next_level))
                        break;
        }
        end_game();
        return 0;
}
