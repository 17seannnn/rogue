/* msg.c - message functions */

#include "rogue.h"

#include <stdlib.h>
#include <string.h>

struct message *msg = NULL;

const char msg_more_text[] = "--More--";

void add_msg(const char *text)
{
        struct message **m, *t;
        t = malloc(sizeof(*t));
        strncpy(t->text, text, msgw_col);
        t->next = NULL;
        for (m = &msg; *m; m = &(*m)->next)
                {}
        *m = t;
}

static void del_msg(struct message *del)
{
        struct message **m;
        for (m = &msg; *m != del; m = &(*m)->next)
                {}
        *m = (*m)->next;
        free(del);
}

static int show_msg(const char *text, int start_x)
{
        mvwprintw(msgw, 0, start_x, text);
        wrefresh(msgw);
        return start_x + strlen(text) + 1;
}

void handle_msg()
{
        int c, x;
        wclear(msgw);
        wrefresh(msgw);
        if (!msg)
                return;
        x = show_msg(msg->text, 0);
        del_msg(msg);
        if (!msg)
                return;
        if (x + strlen(msg->text)+1 + strlen(msg_more_text) >= msgw_col) {
                show_msg(msg_more_text, x);
                do
                        c = wgetch(msgw);
                while (c != ' ');
                handle_msg();
        } else {
                x = show_msg(msg->text, x);
                del_msg(msg);
                if (msg) {
                        show_msg(msg_more_text, x);
                        do
                                c = wgetch(msgw);
                        while (c != ' ');
                        handle_msg();
                }
        }
}
