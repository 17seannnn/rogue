/* msg.c - message functions */

#include "rogue.h"

#include <stdlib.h>
#include <string.h>

struct message *msg = NULL;

static const char msg_more[] = "--More--";
const char msg_nevermind[] = "Never mind.";

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

void append_msg(const char *text)
{
        struct message *m;
        for (m = msg; m && m->next; m = m->next)
                {}
        if (!msg)
                return;
        strncat(m->text, text, msgw_col-1);
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
        int first = 1, x = 0;
        wclear(msgw);
        wrefresh(msgw);
        while (msg) {
                if (first || x + strlen(msg->text)+1 +
                             strlen(msg_more) <= msgw_col) {
                        x = show_msg(msg->text, x);
                        del_msg(msg);
                        if (first)
                                first = 0;
                } else {
                        show_msg(msg_more, x);
                        wait_ch(' ');
                        x = 0;
                        wclear(msgw);
                        wrefresh(msgw);
                }
        }
}
