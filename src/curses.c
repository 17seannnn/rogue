/* curses.c - curses control funcs and vars */

#include <curses.h>

#include "rogue.h"

WINDOW *msgw, *gamew, *infow, *invw;

void init_curses()
{
        initscr();
        if (has_colors()) {
                use_default_colors();
                start_color();
        }
        noecho();
        cbreak();
        curs_set(0);
        clear();
        msgw  = newwin(msgw_row,  msgw_col,  msgw_srow,  msgw_scol);
        infow = newwin(infow_row, infow_col, infow_srow, infow_scol);
        gamew = newwin(gamew_row, gamew_col, gamew_srow, gamew_scol);
        invw  = newwin(invw_row,  invw_col,  invw_srow,  invw_scol);
        keypad(gamew, 1);
        timeout(-1);
}

void end_curses()
{
        endwin();
}

int set_pair(const struct color *c)
{
	int n;
	if (c->fg == -1 && c->bg == -1)
		return 0;
	n = abs_int(c->bg)*8 + abs_int(c->fg) + 1;
	/* Make sure that we don`t rewrite pair without default colors */
	if (c->fg == -1 || c->bg == -1)
		n += 64;
	init_pair(n, c->fg, c->bg);
	return n;
}

void wait_ch(int c)
{
        int t;
        do
                t = wgetch(gamew);
        while (t != c);
}
