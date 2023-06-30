CC=cc
CFLAGS=-O3
LIBS=-lcurses
OUT=druid
COMP=$(CC) $(CFLAGS) -c

all: miv.o
	$(CC) *.o $(LIBS) -o $(OUT)

miv.o: druid.c config.h
	$(COMP) druid.c

config.h:
	cp config.def.h config.h

clean:
	rm *.o
	rm $(OUT)

