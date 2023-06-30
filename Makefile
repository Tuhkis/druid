CC=cc
CFLAGS=-O1
LIBS=-lcurses
OUT=druid
COMP=$(CC) $(CFLAGS) -c

all: miv.o
	$(CC) *.o $(LIBS) -o $(OUT)

miv.o: druid.c
	$(COMP) druid.c

clean:
	rm *.o
	rm $(OUT)

