/* curses.c - curses control funcs and vars */

#include <curses.h>

#include "rogue.h"

WINDOW *msgw, *gamew, *infow, *invw;

void init_curses()
{
        initscr();
	if (has_colors())
		start_color();
        noecho();
        cbreak();
        curs_set(0);
        clear();
        msgw  = newwin(msgw_row,  msgw_col,  msgw_srow,  msgw_scol);
        infow = newwin(infow_row, infow_col, infow_srow, infow_scol);
        gamew = newwin(gamew_row, gamew_col, gamew_srow, gamew_scol);
        invw  = newwin(invw_row,  invw_col,  invw_srow,  invw_scol);
        if (has_colors()) {
                start_color();
                use_default_colors();
        }
        keypad(gamew, 1);
        timeout(-1);
}

void end_curses()
{
        endwin();
}

int create_pair(int fg, int bg)
{
	int n = bg * 8 + fg;
	init_pair(n, fg, bg);
	return n;
}

void wait_ch(int c)
{
        int t;
        do
                t = wgetch(gamew);
        while (t != c);
}
