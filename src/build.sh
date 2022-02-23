#!/bin/sh

gcc -ansi -pedantic -Wall -g -Og -lncurses -DDEBUG -c math.c curses.c room.c path.c level.c fov.c cmd.c hunter.c
gcc -ansi -pedantic -Wall -g -Og -lncurses -DDEBUG math.c curses.o room.o path.o level.o fov.o cmd.o hunter.o main.c -o rogue
