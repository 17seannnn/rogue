/* curses.c - curses control funcs and vars */

#include <curses.h>

#include "rogue.h"

WINDOW *msgw, *gamew, *infow, *invw;

void init_curses()
{
        initscr();
        msgw  = newwin(msgw_row, msgw_col, 0, 0);
        gamew = newwin(gamew_row, gamew_col, msgw_row, 0);
        infow = newwin(infow_row, infow_col, msgw_row + gamew_row, 0);
        invw  = newwin(invw_row, invw_col, msgw_row, gamew_col - invw_col);
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

void end_curses()
{
        endwin();
}

void wait_ch(int c)
{
        int t;
        do
                t = wgetch(gamew);
        while (t != c);
}
