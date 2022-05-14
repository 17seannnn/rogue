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

static void end_game(struct creature *h)
{
        free_loot(h->inv);
        end_curses();
#ifdef DEBUG

	fclose(logfile);
#endif
}

static unsigned play_game(struct level *l, struct creature *h)
{
        int c;
        unsigned flags = 0;
        handle_fov(l, h, 1);
        show_info(h);
        handle_msg();
        for (;;) {
                if (~flags & again_flag)
                        c = wgetch(gamew);
                do_cmd(c, l, h, &flags);
                if (flags & endgame_flag)
                        break;
                handle_beast(l, h);
                if (flags & again_flag && !msg) {
                        handle_fov(l, h, 0);
                        continue;
                } else
                if (flags & again_flag && msg) {
                        flags ^= again_flag;
                }
                handle_fov(l, h, 1);
                show_info(h);
                handle_msg();
        }
        return flags;
}

int main()
{
	int reinit = 1;
        unsigned flags;
        struct creature h;
        struct level l;
        init_game();
        for (;;) {
		init_level(&l, &h, reinit);
                flags = play_game(&l, &h);
                end_level(&l);
                if (flags & endgame_flag)
                        break;
		reinit = 0;
        }
        end_game(&h);
        return 0;
}
