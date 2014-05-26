CC=gcc
LIBS=-lm
OUT=./bin/

battery: main.c main.h battery.c battery.h output.c output.h
	mkdir $(OUT)
	$(CC) -o $(OUT)battery main.c battery.c output.c $(LIBS)

debug: main.c main.h battery.c battery.h output.c output.h
	mkdir $(OUT)
	$(CC) -g -o $(OUT)battery main.c battery.c output.c $(LIBS)
