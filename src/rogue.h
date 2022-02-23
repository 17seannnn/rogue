#ifndef ROGUE_H
#define ROGUE_H

#include <curses.h>

#ifdef DEBUG
#include <stdio.h>
extern FILE *logfile;
#endif

/* curses */
enum {
        msgw_col  = 80,
        msgw_row  = 1,
        gamew_col = 80,
        gamew_row = 22,
        infow_col = 80,
        infow_row = 1
};

/* room */
struct room {
        int ch_idx, no_idx;
        int tl_x, tl_y, br_x, br_y;
        int depth;
        struct room *parent, *left, *right;
};

/* path */
struct path {
        int cur_x, cur_y;
        struct path *next;
};

struct door {
        int cur_x, cur_y;
        struct room *owner;
        struct door *next;
};

/* level */
struct level {
        int depth;
        struct room *r;
        struct path *p;
        struct door *d;
};

/* hunter */
struct hunter {
        int symb;
        int cur_x, cur_y;
};

/* curses */
extern WINDOW *msgw, *gamew, *infow;

/* math */
int abs_int(int i);

/* curses */
void init_curses();
void end_curses();

/* room */
int init_room(struct room **r);
void free_room(struct room *r);
int room_len(const struct room *r, char dir);
struct room *get_room_by_idx(const struct room *r, int ch_idx, int no_idx);
struct room *get_room_by_coord(const struct room *r, int x, int y);
int is_room(const struct room *r, int x, int y);
int is_wall(struct room *r, int x, int y);
void show_rooms(const struct room *r, const struct door *d);

/* path */
int is_path(struct path *p, int x, int y);
void init_path(struct level *l);
void free_path(struct path *p);
void show_path(struct path *p);
int is_door(struct door *d, int x, int y);
void free_door(struct door *d);

/* level */
int is_ok(int x, int y);
int is_empty(struct level *l, int x, int y);
void init_level(struct level *l);
void end_level(struct level *l);

/* hunter */
void init_hunter(struct hunter *h);
void show_hunter(const struct hunter *h);

/* fov */
void handle_fov(const struct hunter *h, const struct level *l);

/* cmd */
void do_cmd(int c, struct hunter *h);

#endif
