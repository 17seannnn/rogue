rogue-like game
===============

*Working only on Unix-like systems*

## Levels

![First level](img/1lvl.png)
![Third level](img/3lvl.png)
![Fourth level](img/4lvl.png)
![Fifth level](img/5lvl.png)

## Commands

| Command | Key |
|---|---|
| Movement  | hjkl, HJKL |
| Inventory | i, * |
| Drop      | d |
| Wield     | w |
| Wear      | W |
| Quaff     | q |
| Go next   | > |

## Symbols

| Symbol | Meaning |
|---|---|
| @ | playable character |
| B | beast, unfriendly npc |
| : | loot |
| # | wall |
| + | door |
| " | path |
| > | thing that move to next level |
| < | thing you came from |

## Dependencies

* C compiler
* make
* ncurses

## Build

``` bash
cd src
make
./rogue
```
