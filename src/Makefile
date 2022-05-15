BINNAME = rogue
SRCMODULES = math.c curses.c room.c path.c loot.c inv.c creature.c\
             hunter.c beast.c level.c msg.c fov.c fight.c cmd.c save.c
OBJMODULES = $(SRCMODULES:.c=.o)
CFLAGS = -g -Wall
LDFLAGS = -lncurses
RM = rm

ifeq (debug, $(MAKECMDGOALS))
	CFLAGS += -DDEBUG
endif

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

debug install: main.c $(OBJMODULES)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $(BINNAME)

ifneq (clean, $(MAKECMDGOALS))
-include deps.mk
endif

deps.mk: $(SRCMODULES)
	$(CC) -MM $^ > $@

clean:
	$(RM) -f *.o $(BINNAME) deps.mk
