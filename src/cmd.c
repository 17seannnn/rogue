/* cmd.c - hunter`s cmd control */

#include "rogue.h"

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

void do_cmd(int c, struct hunter *h)
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
