/* rogue.h - contains all headers */

#ifndef ROGUE_H
#define ROGUE_H

#include <curses.h>

#ifdef DEBUG
#include <stdio.h>
extern FILE *logfile;
#endif

/* curses */
enum {
        program_col = 80,
        program_row = 24,
        msgw_col    = program_col,
        msgw_row    = 1,
        msgw_scol   = 0,
        msgw_srow   = 0,
        infow_col   = 80,
        infow_row   = 1,
        infow_scol  = 0,
        infow_srow  = program_row-1,
        gamew_col   = program_col-2,
        gamew_row   = program_row - (msgw_row + infow_row) - 2,
        gamew_scol  = 1,
        gamew_srow  = msgw_row + 1,
        invw_col    = program_col / 2,
        invw_row    = gamew_row,
        invw_scol   = program_col - invw_col,
        invw_srow   = gamew_srow
};

/* common */
enum {
        key_escape = 27,

        side_northwest = 0,
        side_north,
        side_northeast,
        side_east,
        side_southeast,
        side_south,
        side_southwest,
        side_west,

        seen_flag = 0x8000
};

/* room */
enum {
        wall_symb   = '#',
        door_symb   = '+',
        ground_symb = '.'
};

/* path */
enum {
        path_symb = '"'
};

/* loot */
enum {
        type_weapon = 0,
        type_armor,
        type_poition,
        type_key,

        type_key_level = 0,

        weapon_debug = 0,

        armor_debug = 0,

        poition_debug = 0,

        key_debug = 0,

        loot_symb = ':'
};

/* inv */
enum {
        max_inv = 16
};

/* creature */
enum {
        saw_hunter_flag = 0x0001,

        cast_hunter = 0,
        cast_beast
};

/* cmd */
enum {
        again_flag      = 0x0001,
        next_level_flag = 0x0002
};

/* common */
struct coord {
        int x, y;
};

/* room */
struct room {
        int ch_idx, no_idx;
        struct coord tl, br;
        int depth;
        struct room *parent, *left, *right;
        unsigned flags;
};

/* path */
struct path {
        struct coord pos;
        struct path *next;
        unsigned flags;
};

struct door {
        struct coord pos;
        struct room *owner;
        struct door *next;
        unsigned flags;
};

/* loot */
struct loot {
        const char *name;
        int type, val;
};

struct loot_list {
        int idx;
        struct coord pos;
        const struct loot *l;
        struct loot_list *next;
        unsigned flags;
};

/* creature */
struct creature {
        int cast, symb;
        struct coord pos;
        int fov, hp, dmg;
        int blood;
        int exp, level;
        const struct loot_list *weapon, *armor;
        struct loot_list *inv;
        unsigned flags;
};

/* beast */
struct beast {
        struct creature c;
        struct beast *next;
};

/* level */
struct point {
        int symb;
        struct coord pos;
        unsigned flags;
};

struct level {
        int depth;
        struct point start, end;
        struct room *r;
        struct path *p;
        struct door *d;
        struct beast *b;
        struct loot_list *l;
};

/* msg */
struct message {
        char text[msgw_col];
        struct message *next;
};

extern struct message *msg;
extern const char msg_nevermind[];

/* curses */
extern WINDOW *msgw, *infow, *gamew, *invw;

/* loot */
extern const struct loot weapon_list[];
extern const struct loot armor_list[];
extern const struct loot poition_list[];
extern const struct loot key_list[];

/* math */
int abs_int(int i);
double power(int x, int y);

/* curses */
void init_curses();
void end_curses();
void wait_ch();

/* room */
int room_len(const struct room *r, char dir);
void free_room(struct room *r);
int init_room(struct room **r);
struct room *get_room_by_idx(const struct room *r, int ch_idx, int no_idx);
struct room *get_room_by_coord(const struct room *r, int x, int y);
int is_room(const struct room *r, int x, int y);
int is_one_room(const struct room *r, struct coord pos1, struct coord pos2);
int is_wall(const struct room *r, int x, int y);
void show_room(const struct room *r, const struct door *d);
void show_rooms(const struct room *r, const struct door *d);

/* path */
struct path *get_path_by_coord(const struct path *p, int x, int y);
int is_path(const struct path *p, int x, int y);
int is_door(const struct door *d, int x, int y);
void init_path(struct level *l);
void free_path(struct path *p);
void free_door(struct door *d);
void show_path(const struct path *p);

/* loot */
void add_loot(struct loot_list **ll, const struct loot *l,
              int x, int y, unsigned flags);
void del_loot(struct loot_list **l, struct loot_list *del);
void init_loot(struct level *l, const struct creature *h);
void free_loot(struct loot_list *l);
void show_loot(const struct loot_list *l);
struct loot_list *get_loot_by_coord(const struct loot_list *l, int x, int y);
void try_loot(struct level *l, struct creature *h, int side);

/* inv */
void show_inv(const struct creature *h);
void drop(struct loot_list **ll, struct creature *h);
void wield(struct creature *h);
void wear(struct creature *h);
void quaff(struct creature *h);

/* creature */
void show_creature(const struct creature *c);
int search_creature(const struct level *l, struct creature *c1,
                                           const struct creature *c2);
int move_creature(const struct level *l, const struct creature *h,
                  struct creature *c, int side);

/* hunter */
int is_hunter(const struct creature *h, int x, int y);
void init_hunter(struct creature *h, const struct level *l);
void show_info(const struct creature *h);
void try_move(struct level *l, struct creature *h, int c, unsigned *flags);
int try_attack_beast(const struct creature *h, struct beast *b, int side);
void go_next(const struct level *l, struct creature *h, unsigned *flags);

/* beast */
int is_beast(const struct beast *b, int x, int y);
void init_beast(struct level *l, const struct creature *h);
void free_beast(struct beast *b);
void handle_beast(struct level *l, struct creature *h);

/* level */
int is_ok(int x, int y);
int is_empty(const struct level *l, int x, int y);
int is_stop(const struct level *l, int x, int y);
int can_move(const struct level *l, const struct creature *h, int x, int y);
int can_side(const struct level *l, int side, int x, int y);
int try_side(const struct level *l, int side, int x, int y);
int get_char_side(int c);
void get_side_diff(int side, int *dx, int *dy);
void init_level(struct level *l, struct creature *h);
void end_level(struct level *l);
void show_point(struct point p);

/* msg */
void add_msg(const char *text);
void append_msg(const char *text);
void handle_msg();

/* fov */
void handle_fov(struct level *l, const struct creature *h);

/* fight */
int attack(const struct creature *a, struct creature *d);

/* cmd */
void do_cmd(int c, struct creature *h, struct level *l, unsigned *flags);

#endif
