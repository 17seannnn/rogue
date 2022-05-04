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
	add_loot(&h->inv, &poition_list[2], 0, 0, 0);
        for (;;) {
                if (!(flags & again_flag)) {
                        c = wgetch(gamew);
                        if (c == key_escape)
                                break;
                }
                handle_hunter(h);
                do_cmd(c, h, l, &flags);
                handle_beast(l, h);
                if (flags & next_level_flag)
                        break;
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
        unsigned flags;
        struct creature h;
        struct level l;
        init_game();
        for (;;) {
                init_level(&l, &h);
                flags = play_game(&l, &h);
                end_level(&l, &h);
                if (!(flags & next_level_flag))
                        break;
        }
        end_game();
        return 0;
}
