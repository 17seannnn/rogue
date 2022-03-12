#!/bin/sh

NAME=rogue
CC=gcc

case $1 in
        install)
        gcc -ansi -pedantic -Wall -Wextra -g -Og -lncurses -DDEBUG -c math.c curses.c room.c path.c loot.c creature.c hunter.c beast.c level.c msg.c fov.c fight.c cmd.c
        gcc -ansi -pedantic -Wall -Wextra -g -Og -lncurses -DDEBUG math.c curses.o room.o path.o loot.o creature.c hunter.o beast.o level.o msg.o fov.o fight.o cmd.o main.c -o rogue;;
        clean)
                rm *.o $NAME log;;
        * | --help) echo "\
Usage:
build.sh [COMMAND/--OPTION]

Commands:
        install    install this package
        clean      clean src
        help       show help";;
esac
