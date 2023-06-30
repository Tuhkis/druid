CC=cc
CFLAGS=-O3
LIBS=-lcurses
OUT=miv
COMP=$(CC) $(CFLAGS) -c

all: miv.o
	$(CC) *.o $(LIBS) -o $(OUT)

miv.o: miv.c
	$(COMP) miv.c

clean:
	rm *.o
	rm $(OUT)

