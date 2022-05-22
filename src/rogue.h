/* rogue.h - contains all headers */

#ifndef ROGUE_H
#define ROGUE_H

#include <curses.h>

#include <stdio.h>
extern FILE *logfile;

#define CTRL(C) ((C) & 0x1f)

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
        invw_srow   = gamew_srow,

	start_pair = 1,
	end_pair,
	hor_wall_pair,
	ver_wall_pair,
	door_pair,
	ground_pair,
	path_pair,
	loot_pair,

	fov_start_pair,
	fov_end_pair,
	fov_path_pair,
	fov_loot_pair,

	hunter_pair,

	beast_debug1_pair,
	beast_debug2_pair,
	beast_debug3_pair,
	beast_debug4_pair
};

/* common */
enum {
        key_escape = 27,
	key_ctrl_c = CTRL('c'),

	bufsize = 256,

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
        see_flag = 0x0001
};

/* path */

/* loot */
enum {
	type_blood = 0,
        type_weapon,
        type_armor,
        type_potion,
        type_key,

        type_key_level = 0,

	blood_debug = 0,

        weapon_debug = 0,

        armor_debug = 0,

        potion_debug = 0,

        key_debug = 0
};

/* inv */
enum {
        max_inv = 16
};

/* creature */
enum {
        saw_hunter_flag = 0x0001,

        cast_hunter = 0,
        cast_beast,

	max_creature_level = 15,
	max_blood = 999
};

/* beast */
enum {
	beast_debug1 = 0,
	beast_debug2,
	beast_debug3
};

/* level */
enum {
	max_level_count = 1,

	level_debug = 0
};

/* cmd */
enum {
        again_flag    = 0x0001,
        endgame_flag  = 0x0002,
        endlevel_flag = 0x0004,
	isnext_flag   = 0x0008
};

/* curses */
struct color {
	int fg, bg;
};

/* common */
struct coord {
        int x, y;
};

struct linked_coord {
        struct coord pos;
        struct linked_coord *next;
};

/* room */
struct room {
        int ch_idx, no_idx;
        struct coord tl, br;
        int depth;
        struct room *parent, *left, *right;
        struct linked_coord *seen_walls;
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
        char name[bufsize];
        int type, val;
};

struct loot_list {
        int idx;
        struct coord pos;
        struct loot l;
        struct loot_list *next;
        unsigned flags;
};

/* creature */
struct creature {
        int cast;
	char name[bufsize];
	int symb, color_pair;
	struct color clr;
        struct coord pos;
        int fov, hp, max_hp, dmg, miss;
        int blood, exp, level;
        struct loot_list *weapon, *armor;
        struct loot_list *inv;
	int buff_loot_chance[3], buff_beast_chance[4];
        unsigned flags;
};

/* beast */
struct beast {
        struct creature c;
        struct beast *next;
};

/* level */
struct point {
        struct coord pos;
        unsigned flags;
};

struct level_type {
	/* Number of location */
	int location;
	/* Recommend depth */
	int depth;
	/* Limits */
	int max_loot_count, max_beast_count;
	/*
	 * 1-3 are quality of loot:
	 *   1: weak
	 *   2: default
	 *   3: strong
	 */
	int loot_chance[3];
	/*
	 * 1-4 show how powerful will be beast
	 *   1: weak
	 *   2: default
	 *   3: strong
	 *   4: boss
	 */
	int beast_chance[4];
	/* Symbols for this level */
	int start_symb, end_symb, hor_wall_symb, ver_wall_symb,
            door_symb, ground_symb, path_symb, loot_symb;
	/* Colors */
	struct color start_color, end_color, hor_wall_color, ver_wall_color,
                     door_color, ground_color, path_color, loot_color;
	/* Colors for seen objects */
	struct color fov_start_color, fov_end_color, fov_path_color,
	             fov_loot_color;
};

struct level {
	/* Depth in level is actual depth */
	int depth, count;
        const struct level_type *lt;
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

/* msg */
extern struct message *msg;
extern const char msg_nevermind[];

/* curses */
extern WINDOW *msgw, *infow, *gamew, *invw;

/* loot */
extern const struct loot blood_list[];
extern const struct loot weapon_list[];
extern const struct loot armor_list[];
extern const struct loot potion_list[];
extern const struct loot key_list[];

/* beast */
extern const struct creature beast_list[];

/* level */
extern const struct level_type level_list[];
extern int start_symb, end_symb, hor_wall_symb, ver_wall_symb,
           door_symb, ground_symb, path_symb, loot_symb;
extern int fov_start_symb, fov_end_symb, fov_path_symb, fov_loot_symb;

/* common */
int is_linked_coord(struct linked_coord *lc, int x, int y);
void add_linked_coord(struct linked_coord **lc, int x, int y);
void free_linked_coord(struct linked_coord *lc);

/* math */
int abs_int(int i);
double power(int x, int y);

/* curses */
void init_curses();
void end_curses();
void set_pair(int n, const struct color *c);
void wait_ch();

/* room */
int room_len(const struct room *r, char dir);
void free_room(struct room *r);
int init_room(struct level *l);
struct room *get_room_by_idx(const struct room *r, int ch_idx, int no_idx);
struct room *get_room_by_coord(const struct room *r, int x, int y);
struct room *get_random_room(const struct room *r);
int is_room(const struct room *r, int x, int y);
int is_one_room(const struct room *r, struct coord pos1, struct coord pos2);
int is_wall(const struct room *r, int x, int y);
void show_room(struct room *r, const struct door *d, int ground);
void show_rooms(struct room *r, const struct door *d);
int is_seen_wall(const struct linked_coord *sw, int x, int y);
void show_seen_walls(const struct room *r, const struct door *d);

/* path */
struct path *get_path_by_coord(const struct path *p, int x, int y);
int is_path(const struct path *p, int x, int y);
int is_door(const struct door *d, int x, int y);
void init_path(struct level *l);
void free_path(struct path *p);
void free_door(struct door *d);
void show_path(const struct path *p);

/* loot */
struct loot_list *get_loot_by_coord(const struct loot_list *l, int x, int y);
int is_loot(struct loot_list *l, int x, int y);
void add_loot(struct loot_list **ll, const struct loot *l,
              int x, int y, unsigned flags);
void del_loot(struct loot_list **l, struct loot_list *del);
void init_loot(struct level *l, const struct creature *h);
void free_loot(struct loot_list *l);
void show_loot(const struct loot_list *l);
int count_loot(const struct loot_list *ll);
void try_loot(struct level *l, struct creature *h, int side);

/* inv */
void show_inv(struct level *l, const struct creature *h);
void drop(struct level *l, struct creature *h);
void wield(struct level *l, struct creature *h);
void wear(struct level *l, struct creature *h);
void quaff(struct level *l, struct creature *h);

/* creature */
void show_creature(const struct creature *c);
int search_creature(const struct level *l, struct creature *c1,
                                     const struct creature *c2);
int move_creature(const struct level *l, const struct creature *h,
                  struct creature *c, int side);
int add_health(struct creature *c, int val);
void add_blood(struct level *l, struct creature *c, int val);
int get_max_exp(int level);
void add_exp(struct creature *c, int val);

/* hunter */
int is_hunter(const struct creature *h, int x, int y);
void init_hunter(const struct level *l, struct creature *h, int hs);
void show_info(const struct creature *h);
void try_move(struct level *l, struct creature *h, int c, unsigned *flags);
void try_attack_beast(struct level *l, struct creature *h,
		      struct beast *b, int side);
void handle_hunter(struct creature *h);
void go_next(struct level *l, struct creature *h, unsigned *flags);

/* beast */
int is_beast(const struct beast *b, int x, int y);
void init_beast(struct level *l, const struct creature *h);
void free_beast(struct beast *b);
int count_beast(const struct beast *b);
struct beast *get_random_beast(const struct beast *b);
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
void end_level(struct level *l, struct creature *h, int is_next);

/* msg */
void add_msg(const char *fmt, ...);
void append_msg(const char *fmt, ...);
void handle_msg();

/* fov */
void handle_fov(struct level *l, const struct creature *h, int refresh);
void redraw_screen(struct level *l, const struct creature *h);

/* fight */
void attack(struct level *l, struct creature *a, struct creature *d);

/* cmd */
void do_cmd(int c, struct level *l, struct creature *h, unsigned *flags);

/* save */
void set_save_fn(const char *fn);
void save_game(const struct level *l, const struct creature *h);
int load_game(struct level *l, struct creature *h);
void del_save();

#endif
