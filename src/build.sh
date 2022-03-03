#!/bin/sh

NAME=rogue
CC=gcc

case $1 in
        install)
        gcc -ansi -pedantic -Wall -Wextra -g -Og -lncurses -DDEBUG -c math.c curses.c room.c path.c creature.c hunter.c beast.c level.c fov.c cmd.c
        gcc -ansi -pedantic -Wall -Wextra -g -Og -lncurses -DDEBUG math.c curses.o room.o path.o creature.c hunter.o beast.o level.o fov.o cmd.o main.c -o rogue;;
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
