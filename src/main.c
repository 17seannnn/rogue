/* main.c - main game functions */

#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "rogue.h"

static const char msg_gameover[] = "You died! Press space to continue...";

FILE *logfile;

static void init_game()
{
	char save_fn[bufsize];
	strncpy(save_fn, getenv("HOME"), bufsize);
	strncat(save_fn, "/.rogue.save", bufsize-1);
	set_save_fn(save_fn);
        logfile = fopen("log", "w");
        srand(time(NULL));
        init_curses();
}

static void end_game()
{
        end_curses();
	fclose(logfile);
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
                if (flags & endgame_flag || flags & endlevel_flag)
                        break;
                handle_beast(l, h);
		if (h->hp <= 0) {
			flags |= endgame_flag;
			del_save();
			add_msg(msg_gameover);
			handle_msg();
			show_info(h);
			wait_ch(' ');
			break;
		}
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
                end_level(&l, &h, flags & endlevel_flag);
                if (flags & endgame_flag)
                        break;
        }
        end_game();
        return 0;
}
